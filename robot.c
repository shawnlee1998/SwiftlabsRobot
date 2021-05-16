#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define COMMAND_LENGTH 7
#define COMMAND_PREFIX_LENGTH 3

int interval;

char *buffer;
int maxsize;
int front = 0;
int rear = -1;
int elements = 0;

bool running = true;

bool isQueueEmpty() {
	return elements == 0;
}

bool isQueueFull() {
	// not only check for queue full but also if new command will fit
	return (maxsize-elements <= COMMAND_LENGTH);
}

void queueInsert(char *data, int length) {
	for(int i=0; i<length; i++) {
		if(!isQueueFull()) {
			if(rear == maxsize-1) {
        		rear = -1;
    		}
    		elements++;
    		buffer[++rear]=data[i];
		}
		else {
			printf("Queue max size reached.\n");
		}
	}
}

void removeData(char *buf, int count) {
	for(int i=0; i<count; i++) {
		if(front == maxsize) {
    		front = 0;
		}
		buf[i] = buffer[front++];
		elements--;
	}
}

void interpretCommand(char *command) {
	char data[4];

	for(int i=0; i<sizeof(data)/sizeof(data[0]); i++) {
		data[i] = command[i+COMMAND_PREFIX_LENGTH];
	}

	if(!strcmp(data, "EXIT")) {
		printf("Exited.\n");
		running = false; //Handle thread termination
	}
	else {
		char magArr[2];
		magArr[0] = data[2];
		magArr[1] = data[3];
		int mag = atoi(magArr);
		if(data[0] == 'M') {
			if(data[1] == 'N'){
				printf("Moved north %d steps.\n", mag);
			}
			else if(data[1] == 'S') {
				printf("Moved south %d steps.\n", mag);
			}
			else if(data[1] == 'E') {
				printf("Moved east %d steps.\n", mag);
			}
			else if(data[1] == 'W') {
				printf("Moved west %d steps.\n", mag);
			}
			else {
				printf("Invalid command.\n");
			}
		}
		else {
			printf("Invalid command.\n");
		}
	}
}

void *moveInput(void *args) {
	char temp[maxsize];
	int ret;
	// for(int i=0; i<5; i++) {
	while(running) {
		printf("Input command: \n");
		fgets(temp, maxsize+1, stdin);
		// read(0, temp, maxsize);
		// printf("num of elements in temp is %d\n", strlen(temp)-1);
		// printf("you entered: %s\n", temp);

		char prefix[3];
		for(int i=0; i<COMMAND_PREFIX_LENGTH; i++) {
			prefix[i] = temp[i];
		}

		if(strlen(temp)-1 != COMMAND_LENGTH) {
			printf("Invalid command.\n");
		}
		else if(!strcmp(prefix, "+++")) {
			printf("Invalid command.\n");
		}
		else {
			queueInsert(temp, strlen(temp)-1);
		}
		printf("current buffer: %s\n", buffer);
	}
	printf("thread 1 exited\n");
	pthread_exit(NULL);
}

void *moveOutput(void *args) {
	char command[8];
	// for(int i=0; i<20; i++) {
	while(running) {
		sleep(interval);
		if(elements >= COMMAND_LENGTH) {
			removeData(command, COMMAND_LENGTH);
			command[COMMAND_LENGTH]='\0';
			interpretCommand(command);
			// printf("my command: %s\n", command);
		}
	}
	printf("thread 2 exited\n");
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	if (argc!=3) {
		printf("Expected number of arguments is 2");
		return -1;
	}
	else {
		maxsize = atoi(argv[1]);
		interval = atoi(argv[2]);
		buffer = malloc(maxsize);

		pthread_t producer;
		pthread_t consumer;

		pthread_create(&producer, NULL, moveInput, NULL);
		pthread_create(&consumer, NULL, moveOutput, NULL);
		pthread_join(producer, NULL);
		pthread_join(consumer, NULL);
	}
}