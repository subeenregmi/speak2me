#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <threads.h>
#include <string.h>
#include <stdlib.h>

#include "client_helper.h"

#define MAX_CONNECTIONS 5
#define MAX_NAME_LENGTH 64

thrd_t clients_thread[MAX_CONNECTIONS];
int clients_i = 0;

struct clientsinfo clients;
struct clientsinfo *head = &clients;

int handle_client(void *arg) {

	struct clientsinfo *client = (struct clientsinfo*)arg;

	char welcome[] = "Welcome to subeen's chat!";
	send(client->fd, welcome, strlen(welcome), 0);

	char name_prompt[] = "Enter your name: ";
	send(client->fd, name_prompt, strlen(name_prompt), 0);
	
	char name[1024] = "";
	int bytes = recv(client->fd, name, 1024, 0);

	if (bytes == 0) {
		printf("Client %d has left during name prompt!\n", client->fd);
		remove_client(client);
		return 1;
	}
	if (bytes == -1) {
		printf("Client error!\n");
		printf("client.fd = %d\n", client->fd);
		printf("client.name = %s\n", client->name);
		remove_client(client);
		return 1;
	}
	client->name = name;
	printf("User %s has been succesfully connected!\n", name);

	while (1) {
		char msg[1024] = "";
		int bytes = recv(client->fd, msg, 1024, 0);
		if (bytes == 0) {
			printf("Client %d has left!\n", client->fd);
			remove_client(client);
			break;
		}
		if (bytes == -1) {
			printf("Client error!\n");
			printf("client.fd = %d\n", client->fd);
			printf("client.name = %s\n", client->name);
			remove_client(client);
			break;
		}
		char new_msg[2048] = "";
		printf("%s (fd %d): %s\n", client->name, client->fd, msg);
		sprintf(new_msg, "%s : %s", client->name, msg);
		
		// Send messages to all clients
		struct clientsinfo *node = head;
		while (node != NULL) {
			if (node->fd != client->fd)
				send(node->fd, new_msg, 1024, 0);

			node = node->right;
		}
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
	
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
		perror("setsockopt");
		return 2;
	}

	int sockbind = bind(sockfd, results->ai_addr, results->ai_addrlen);
	if (sockbind == -1) {
		perror("bind");
		return 2;
	}

	int socklisten = listen(sockfd, MAX_CONNECTIONS);
	if (socklisten == -1) {
		perror("listen");
		return 2;
	}

	while (1) {
		struct sockaddr_storage client;
		socklen_t client_sock_size = sizeof client;

		int clientfd = accept(sockfd, (struct sockaddr *)&client, &client_sock_size);
		printf("A new client has joined!\n");

		struct clientsinfo *new = add_client(head, clientfd, "");
	
		thrd_create(&clients_thread[clients_i], handle_client, new);
		thrd_detach(clients_thread[clients_i]);

		clients_i++;
	}
}
