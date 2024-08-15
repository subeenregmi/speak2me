#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"

thrd_t listener;

int listen_to_messages(void *arg) {
	int sockfd = *(int *)arg;
	
	while (1) {
		char msg[1024] = "";
		int bytes = recv(sockfd, msg, 1024, 0);
		if (bytes == 0) {
			printf("Server has disconnected, bye!\n");
			exit(1);
		}
		
		printf("%s\n", msg);
	}

	return 1;
}


int main(void) {
	struct addrinfo hints = generate_socket_hints();
	struct addrinfo *results;

	get_address_info(&hints, &results);
	int sockfd = create_socket(results);
	connect_socket(sockfd, results);

	thrd_create(&listener, listen_to_messages, &sockfd);
	thrd_detach(listener);

	while (1) {
		char msg[1024] = "";
		fgets(msg, sizeof msg, stdin);
		send(sockfd, msg, strlen(msg)-1, 0);
	}
}
