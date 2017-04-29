// Name:  Jairo Vera
// COSC 4330 Spring 2015 Second Assignment
// This is the main program you should use to test your semaphore calls

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// you can add all the includes you need but
// cannot modify the main program
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define MAXHOSTNAME 255
#define PORT_NUM 7169

int sem_create(int initial_value);
int kill_server();
int sem_P(int sem_id);
int sem_V(int sem_id);
int sem_destroy(int sem_id);

int send_buffer(int buffer[]);
int get_reply(int socket);
int talk_to_server(int buffer[]);
int create_client_socket();

int main() {
    int sem1, mutex; //semaphores
    int pid; // process ID
    if ((sem1 = sem_create(3)) < 0) {
         printf("Cannot create semaphore sem1.\n");
         _exit(1);
    }

    if ((mutex = sem_create(1)) < 0) {
         _exit(2);
         printf("Cannot create semaphore mutex.\n");
    }
    // basic checks
    sem_P(sem1);
    sem_P(sem1);
    sem_P(sem1);
    printf("Completed three P() operations on sem1.\n");
    sem_V(sem1);
    sem_V(sem1);
    sem_V(sem1);
    printf("Completed three V() operations on sem1.\n");
    if (sem_P(10) >= 0) {
        printf("Server accepted a call to a non-existent semaphore!\n");
    }

    if ((pid = fork()) == 0) {
        // child process
        printf("Child process requests the mutex.\n");
        sem_P(mutex);
        printf("Child process got the mutex.\n");
        sleep(10);
        printf("Child process wants to release the mutex.\n");
        sem_V(mutex);
        printf("Child process released the mutex.\n");
        _exit(0);
    }
    sleep(2);
    printf("Parent process requests the mutex.\n");
    sem_P(mutex);
    printf("Parent process got the mutex.\n");
    sleep(10);
    printf("Parent process wants to release the mutex.\n");
    sem_V(mutex);
    printf("Parent process released the mutex.\n");

    // Ending
    sem_destroy(sem1);
    if (sem_P(sem1) >= 0) {
        printf("Semaphore sem1 was not properly destroyed!\n");
    }
    sem_destroy(mutex);
    if (sem_P(mutex) >= 0) {
        printf("Semaphore mutex was not properly destroyed!\n");
    }
    kill_server();
} // main

int sem_create(int initial_value) {
	printf("Asking server to create sempahore with initial value %i\n", initial_value);

	int buffer[3];
	buffer[0] = htonl(0);					// op_code = 0
	buffer[1] = htonl(-1);					// sem_id  = -1
	buffer[2] = htonl(initial_value);		// value   = initial_value

	int sem_id = talk_to_server(buffer);
	if (sem_id >= 0) {
		printf("Server successfully created a semaphore with id = %i ", sem_id);
		printf("and initial value %i\n\n", initial_value);
	}

	return sem_id;
}

int kill_server() {
	puts("Asking the server to kill itself");

	int buffer[3];
	buffer[0] = htonl(1);		// op_code = 1
	buffer[1] = htonl(-1);		// sem_id  = -1
	buffer[2] = htonl(-1);		// value   = -1

	int response = talk_to_server(buffer);
	puts("Server is dead");
	return 0;
}

int sem_P(int sem_id) {
	printf("Asking server to call sem_P on sempahore with id = %i\n", sem_id);

	int buffer[3];
	buffer[0] = htonl(2);		// op_code = 2
	buffer[1] = htonl(sem_id);	// sem_id  = sem_id
	buffer[2] = htonl(-1);		// value   = -1

	int response = talk_to_server(buffer);
	if (response == 0)
		printf("Semaphore with id = %i was decremented\n\n", sem_id);
	else
		printf("Semaphore with id = %i was not decremented\n\n", sem_id);

	return response;
}

int sem_V(int sem_id) {
	printf("Asking server to call sem_V on sempahore with id = %i\n", sem_id);

	int buffer[3];
	buffer[0] = htonl(3);		// op_code = 3
	buffer[1] = htonl(sem_id);	// sem_id  = sem_id
	buffer[2] = htonl(-1);		// value   = -1

	int response = talk_to_server(buffer);
	if (response == 0)
		printf("Semaphore with id = %i was incremented.\n\n", sem_id);
	else
		printf("Semaphore with id = %i was not incremented.\n\n", sem_id);
	return response;
}

int sem_destroy(int sem_id) {
	printf("Asking server to destroy sempahore with id = %i\n", sem_id);

	int buffer[3];
	buffer[0] = htonl(4);		// op_code = 4
	buffer[1] = htonl(sem_id);	// sem_id  = sem_id
	buffer[2] = htonl(-1);		// value   = -1

	int response = talk_to_server(buffer);
	if (response == 0)
		printf("Semaphore with id = %i was destroyed.\n\n", sem_id);
	return response;
}

int talk_to_server(int buffer[]){
	int socket = send_buffer(buffer);
	if (socket == -1)
		return -1;

	puts("Waiting for server reply . . .");
	int reply = get_reply(socket);
	if (reply == -1)
		return -1;

	close(socket);
	return reply;
}

int send_buffer(int buffer[]){
	int socket = create_client_socket();
	if (send(socket, buffer, 3*sizeof(int), MSG_WAITALL) < 0) {
		puts("Error. Could not send buffer to server.");
		return -1;
	}
	return socket;
}

int get_reply(int socket){
	int reply;
	if (recv(socket, &reply, sizeof(int), MSG_WAITALL) < 0){
		puts("Error. Could not get server reply.");
		return -1;
	}
	return ntohl(reply);
}

int create_client_socket(){
	int socket_desc;
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
		puts("Process terminating . . .");
		_exit(EXIT_FAILURE);
	}

	// Prepare the sockaddr_in structure
	server.sin_family = hp->h_addrtype;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT_NUM);

	// Create socket
	if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) == -1){
		puts("Error. Could not create client socket.");
		puts("Process terminating . . .");
		_exit(EXIT_FAILURE);
	}

	// Connect to server
	if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		puts("Could not connect to the server.");
		puts("Process terminating . . .");
		_exit(EXIT_FAILURE);
	}

	return socket_desc;
}
