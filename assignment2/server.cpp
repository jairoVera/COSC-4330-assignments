// Name:  Jairo Vera
// COSC 4330 Spring 2015 Second Assignment
// This is the server program that takes requests from the main2.c client program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <vector>
#include <queue>
#define MAXHOSTNAME 255
#define PORT_NUM 7169
using std::vector;
using std::queue;

class Semaphore {
public:
	Semaphore(int sem_id, int sem_val);

	int get_sem_val();
	void set_sem_val(int sem_val);

	void push_queue(int sock_desc);
	int pop_queue();
	int get_queue_size();

private:
	int sem_val;
	queue<int> processQueue;
};

vector<Semaphore> sem_vector;

int sem_create(int new_socket, int initial_value);
int kill_server(int new_socket);
int sem_P(int new_socket, int sem_id);
int sem_V(int new_socket, int sem_id);
int sem_destroy(int new_socket, int sem_id);

int create_server_socket();

int main () {
	int socket_desc = create_server_socket();
	int loop 		= true;
	int new_socket;

	do {
		puts("Server is live! Waiting for incomming connections");
		if ((new_socket = accept(socket_desc, NULL, NULL)) < 0) {
			if (errno == EINTR)
				continue;
			puts("Could not accept incomming connection");
			puts("Server shutting down . . .");
			exit(EXIT_FAILURE);
		}

		// Receive Buffer
		puts("Server accepted an incomming call.");
		int buffer[3];
		if (recv(new_socket, buffer, 3*sizeof(int), MSG_WAITALL) < 0){
			puts("Error. Could not get the client's message.");
			puts("Server shutting down . . .");
			exit(EXIT_FAILURE);
		}
		int op_code 		= ntohl(buffer[0]);
		int sem_id  		= ntohl(buffer[1]);
		int initial_value   = ntohl(buffer[2]);
		printf("Buffer = {%i, %i, %i}\n", op_code, sem_id, initial_value);

		switch(op_code) {
			case 0:
				sem_create(new_socket, initial_value);
				break;
			case 1:
				kill_server(new_socket);
				loop = false;
				break;
			case 2:
				sem_P(new_socket, sem_id);
				break;
			case 3:
				sem_V(new_socket, sem_id);
				break;
			case 4:
				sem_destroy(new_socket, sem_id);
				break;
			default:
				puts("Error. Server recived unknown op_code.");
				puts("Server shutting down . . .");
				exit(EXIT_FAILURE);
		}
	} while(loop);

	close(socket_desc);
	close(new_socket);
	puts("About to kill the server");
	return 0;
}

int sem_create(int new_socket, int initial_value) {
	int sem_id;
	int size = sem_vector.size();
	puts("Server is about to call sem_create()");

	if (size == 0) {
		// Create first semaphore
		Semaphore sem(0, initial_value);
		sem_vector.push_back(sem);
		sem_id = htonl(0);
		printf("Created semaphore id = %i with initial value %i\n\n", 0, initial_value);

	} else {
		for (int i = 0; i < size; i++) {
			if (sem_vector[i].get_sem_val() == -1) {
				sem_vector[i].set_sem_val(initial_value);
				sem_id = htonl(i);
				printf("Created semaphore id = %i with initial value %i\n\n", i, initial_value);
				break;
			}
		}

		Semaphore sem(size, initial_value);
		sem_vector.push_back(sem);
		sem_id = htonl(size);
		printf("Created semaphore id = %i with initial value %i\n\n", size, initial_value);
	}

	if (send(new_socket, &sem_id, sizeof(int), MSG_WAITALL) < 0) {
		puts("Error. Could not send sem_id to the client.");
		puts("Server terminating . . .");
		exit(EXIT_FAILURE);
	}

    return 1;
}

int kill_server(int new_socket) {
	puts("Server is about to call kill_server()");
	int exit_code = htonl(0);
	if (send(new_socket, &exit_code, sizeof(int), MSG_WAITALL) < 0) {
		puts("Error. Could not tell client that we will kill the server.");
		puts("Servers terminating . . .");
		exit(EXIT_FAILURE);
	}
	return 1;
}

int sem_P(int new_socket, int sem_id) {
	int response = -1;
	int size = sem_vector.size();
	printf("Server is about to call sem_P() on semaphore with id = %i\n", sem_id);

	for (int i = 0; i < size; i++) {
		if (i == sem_id) {
			int sem_val = sem_vector[i].get_sem_val();

			printf("Before sem_P: sem_id = %i AND sem_val = %i\n", sem_id, sem_val);

			if (sem_val > 0) {
				sem_vector[i].set_sem_val(sem_val - 1);
				sem_val = sem_vector[i].get_sem_val();
				response = 0;
				printf("After sem_P: sem_id = %i AND sem_val = %i\n\n", sem_id, sem_val);
				break;
			} else if (sem_val == 0) {
				printf("Because sem_val = 0, the process is placed in the queue.\n\n");
				sem_vector[i].push_queue(new_socket);
				return 1;
			} else if (sem_val < 0)
				break;
		}
	}

	if (response == -1)
		printf("Semaphore with id = %i does not exist. sem_P did nothing.\n\n", sem_id);

	if (send(new_socket, &response, sizeof(int), MSG_WAITALL) < 0) {
		puts("Error. Could not send response to the client.");
		puts("Server terminating . . .");
		exit(EXIT_FAILURE);
	}

	return 1;
}

int sem_V(int new_socket, int sem_id) {
	int response = -1;
	int size = sem_vector.size();
	printf("Server is about to call sem_V() on semaphore with id = %i\n", sem_id);

	for (int i = 0; i < size; i++) {
		if (i == sem_id) {
			int sem_val = sem_vector[i].get_sem_val();
			printf("Before sem_V: sem_id = %i AND sem_val = %i\n", sem_id, sem_val);
			sem_vector[i].set_sem_val(sem_val + 1);

			sem_val = sem_vector[i].get_sem_val();
			printf("After sem_V: sem_id = %i AND sem_val = %i\n\n", sem_id, sem_val);
			response = 0;

			if (send(new_socket, &response, sizeof(int), MSG_WAITALL) < 0) {
				puts("Error. Could not send response to the client.");
				puts("Server terminating . . .");
				exit(EXIT_FAILURE);
			}

			// Check the semaphore queue
			if (sem_vector[i].get_queue_size() >= 1) {
				int new_socket = sem_vector[i].pop_queue();
				printf("Server released a process from sempahore id = %i's queue.\n", sem_id);
				sem_P(new_socket, sem_id);
				return 1;
			}
			else
				return 1;
		}
	}

	if (response == -1)
		printf("Semaphore with id = %i does not exist. sem_V did nothing.\n\n", sem_id);

	if (send(new_socket, &response, sizeof(int), MSG_WAITALL) < 0) {
		puts("Error. Could not send response to the client.");
		puts("Server terminating . . .");
		exit(EXIT_FAILURE);
	}

	return 1;
}

int sem_destroy(int new_socket, int sem_id) {
	int response = -1;
	int size = sem_vector.size();
	printf("Server is about to call sem_destroy() on semaphore with id = %i\n", sem_id);

	for (int i = 0; i < size; i++) {
		if (i == sem_id) {
			sem_vector[i].set_sem_val(-1);
			response = 0;
			printf("Destroyed sempahore with sem_id = %i\n\n", sem_id);
			break;
		}
	}

	if (response == -1)
		printf("Semaphore with id = %i does not exist. Can't destroy it.\n\n", sem_id);

	if (send(new_socket, &response, sizeof(int), MSG_WAITALL) < 0) {
		puts("Error. Could not send response to the client.");
		puts("Server terminating . . .");
		exit(EXIT_FAILURE);
	}

	return 1;
}

int create_server_socket(){
	int    socket_desc;
	struct sockaddr_in server;
	struct hostent *hp;
	char   myname[MAXHOSTNAME+1];

	// Clear our address
	memset(&server, 0, sizeof(struct sockaddr_in));

	// Get host entity
	gethostname(myname, MAXHOSTNAME);
	if ((hp = gethostbyname(myname)) == NULL) {
		herror("gethostbyname(): ");
		puts("Error. The server does not exists?");
		puts("Server shutting down . . .");
		exit(EXIT_FAILURE);
	}

	// Prepare the sockaddr_in structure
	server.sin_family = hp->h_addrtype;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT_NUM);

	// Create socket
	if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) == -1){
		puts("Error. Could not create server socket.");
		puts("Server shutting down . . .");
		exit(EXIT_FAILURE);
	}

	// Bind socket to port
	if ((bind(socket_desc,(struct sockaddr *)&server , sizeof(server))) == -1) {
		puts("Error. Could not bind socket.");
		puts("Server shutting down . . .");
		close(socket_desc);
		exit(EXIT_FAILURE);
	}

	// Listen
	listen(socket_desc, 5);

	return socket_desc;
}

Semaphore::Semaphore(int sem_id, int sem_val){
	this->sem_val = sem_val;
}

int Semaphore::get_sem_val(){
	return sem_val;
}

void Semaphore::set_sem_val(int sem_val){
	this->sem_val = sem_val;
}

void Semaphore::push_queue(int sock_desc){
	processQueue.push(sock_desc);
}

int Semaphore::pop_queue(){
	int front = processQueue.front();
	processQueue.pop();
	return front;
}

int Semaphore::get_queue_size(){
	return static_cast<int>(processQueue.size());
}
