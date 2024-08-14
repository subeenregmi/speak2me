#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <stdlib.h>

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
	int status;

	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *results;

	if ((status = getaddrinfo(NULL, "5001", &hints, &results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}
	
	int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

	if (sockfd == -1) {
		perror("socket");
		return 2;
	}

	int res = connect(sockfd, results->ai_addr, results->ai_addrlen);

	if (res == -1) {
		perror("connect");
		return 2;
	}
	
	thrd_create(&listener, listen_to_messages, &sockfd);
	thrd_detach(listener);

	while (1) {
		char msg[1024] = "";
		fgets(msg, sizeof msg, stdin);
		send(sockfd, msg, strlen(msg)-1, 0);
	}
}
