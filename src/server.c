#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <threads.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#include "utils.h"
#include "messages.h"

#define MAX_CONNECTIONS 5
#define MAX_NAME_LENGTH 64

thrd_t clients_thread[MAX_CONNECTIONS];
int clients_i = 0;

struct clientsinfo clients;
struct clientsinfo *head = &clients;

struct message msgs;

int handle_client(void *arg) {

	struct clientsinfo *client = (struct clientsinfo*)arg;
	client->name = "Anon";
	printf("Anon has joined!\n");

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
		add_message(&msgs, client->name, msg);
		
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

void handle_incoming_client(int fd) {
	while (1) {
		struct sockaddr_storage client;
		socklen_t client_sock_size = sizeof client;

		int clientfd = accept(fd, (struct sockaddr *)&client, &client_sock_size);
		printf("A new client has joined!\n");

		struct clientsinfo *new = add_client(head, clientfd, "");
	
		thrd_create(&clients_thread[clients_i], handle_client, new);
		thrd_detach(clients_thread[clients_i]);

		clients_i++;
	}

}

int main(void) {

	struct addrinfo hints = generate_socket_hints();
	struct addrinfo *results;

	get_address_info(&hints, &results);
	int sockfd = create_socket(results);
	set_port_reuse(sockfd);
	bind_socket(sockfd, results);
	listen_socket(sockfd, MAX_CONNECTIONS);
	handle_incoming_client(sockfd);
}
