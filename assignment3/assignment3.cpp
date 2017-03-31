// Name:  Jairo Vera
// PS ID: 1170501
// COSC 4330 Spring 2015 Third Assignment
// This is the main program that simulates customers in a post office

/**
* To compile the program: g++ *.cpp -lpthread -o a3
* To run the program:     ./a3 < <text file> <num_clerks>
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <queue>
#include "customer.h"
using std::queue;
using std::cout;

void check_argc(int argc);
void check_num_clerks(int num_clerks);
void create_semaphores(int num_clerks);
void create_shared_mem();
void get_input();
void print_results();
void destroy_shared_mem();
void destroy_semaphores();

sem_t *sem_post_office;
sem_t *sem_mutex;

char sem_post_name[]  = "jv_post_office";
char sem_mutex_name[] = "jv_mutex";

long smem_key;
int  smem_id;
int  *p_num_wait;   // pointer to shared memory segment

int  num_clerks;
int  num_customers;

queue<Customer> waiting_queue;

int main(int argc, char **argv) {
    // Get & check number of clerks
    check_argc(argc);
	num_clerks = atoi(argv[1]);
	check_num_clerks(num_clerks);
	
    // Create semaphores & shared memory
    create_semaphores(num_clerks);
    create_shared_mem();

    // Get customers
    get_input();
    
    // Nice display
    int test_clerks;
    sem_getvalue(sem_post_office, &test_clerks);
    printf("************************************\n");
    printf("Dr. Paris's post office is now open!\n");
    printf("He has %i TAs as clerks!\n", test_clerks);
    printf("************************************\n");
    
    // Create children processes
    while (!waiting_queue.empty()) {
        //  Get customer info and pop queue
        std::string name  = waiting_queue.front().get_name();
        int arrival_delay = waiting_queue.front().get_elapsed_sec();
        int service_time  = waiting_queue.front().get_process_sec();
        waiting_queue.pop();

        // Delay cutomer's arrival
        sleep(arrival_delay);

        int pid;
        if ((pid = fork()) == 0) {
            // Child Process
            cout<< name <<" arrives at the post office.\n";
            
            // Get sem value
            int idle_clerks;
            sem_getvalue(sem_post_office, &idle_clerks);
            printf("Number of idle clerks: %i\n", idle_clerks);

            // If all clerks are busy
            if (idle_clerks == 0) {
                cout<< name <<" is waiting in line.\n";
                sem_wait(sem_mutex);
                (*p_num_wait)++;
                sem_post(sem_mutex);
            }

            // Serve customer
            sem_wait(sem_post_office);
            cout<< name <<" is getting helped.\n";
            sleep(service_time);
            
            // Customer finishes and leaves
            cout<<"\nIt took " << service_time << " time units to help " << name <<".\n\n";
            cout<< name <<" is leaving the post office.\n";
            sem_post(sem_post_office);
            
            sem_getvalue(sem_post_office, &idle_clerks);
            printf("Number of idle clerks: %i\n", idle_clerks);
            _exit(EXIT_SUCCESS);
        } // End of child
    }
    
    // Wait for the children to be done
    for (int i = 0; i < num_customers; i++)
        wait(0);

    // Print results
    print_results();

    // Destroy semaphores & shared memory
    destroy_shared_mem();
    destroy_semaphores();

	return 0;
}

void check_argc(int argc) {
    if (argc != 2) {
        puts("We only need 1 extra command line arguments.");
        puts("Pass only the number of clerks in the integer range [1,16].");
        puts("Program terminating . . . ");
        exit(EXIT_FAILURE);
    }
}

void check_num_clerks(int num_clerks) {
    if (num_clerks <= 0) {
        puts("Post office needs clerks.");
        puts("Program terminating . . . ");
        exit(EXIT_FAILURE);
    } else if (num_clerks > 16) {
        puts("Too many clerks.");
        puts("Program terminating . . . ");
        exit(EXIT_FAILURE);
    }
}

void create_semaphores(int num_clerks) { 
    sem_post_office = sem_open(sem_post_name, O_CREAT, 0600, num_clerks);
    if (sem_post_office == SEM_FAILED) {
        perror("Could not create the post office semaphore: ");
        puts("Program terminating . . . ");
        sem_unlink(sem_post_name);
        exit(EXIT_FAILURE);
    }

    sem_mutex = sem_open(sem_mutex_name, O_CREAT, 0600, 1);
    if (sem_mutex == SEM_FAILED) {
        perror("Could not create the mutex semaphore: ");
        puts("Program terminating . . . ");
        sem_close(sem_post_office);
        sem_unlink(sem_post_name);
        sem_unlink(sem_mutex_name);
        exit(EXIT_FAILURE);
    }
}

void create_shared_mem() {
    smem_key = 34867497;
    smem_id  = shmget(smem_key, sizeof(int), 0600 | IPC_CREAT);

    if (smem_id == -1) {
        perror("Could not create shared memory: ");
        puts("Program terminating . . . ");
        destroy_semaphores();
        exit(3);
    }

    p_num_wait = (int*)shmat(smem_id, NULL, 0);
    if (p_num_wait == NULL) {
        perror("Could not get address of shared memory.");
        puts("Program terminating . . . ");
        destroy_shared_mem();
        destroy_semaphores();
        exit(24);
    }

    *p_num_wait = 0;
}

void get_input() {
    std::string name;
    int elapsed_sec;
    int process_sec;

    using std::cin;
    while (cin>>name && cin>>elapsed_sec && cin>>process_sec) {
        Customer customer(name, elapsed_sec, process_sec);
        waiting_queue.push(customer);
    }

    num_customers = waiting_queue.size();
}

void print_results() {
	printf("\nHere are the termination results!\n");
	printf("Number of customers serviced:          %i\n", num_customers);
	printf("Number of customers that did not wait: %i\n", num_customers - *p_num_wait);
	printf("Number of customers that did wait:     %i\n", *p_num_wait);
}

void destroy_shared_mem() {
    shmdt(p_num_wait);
    shmctl(smem_id, 0, IPC_RMID);
}

void destroy_semaphores() {
    sem_close(sem_post_office);
    sem_close(sem_mutex);
    
    int post_unlink  = sem_unlink(sem_post_name);
    int mutex_unlink = sem_unlink(sem_mutex_name);

    if (post_unlink == -1 || mutex_unlink == -1) {
        puts("There was an error closing a semaphore.");
        puts("Program terminating . . . ");
        exit(EXIT_FAILURE);
    }
}
