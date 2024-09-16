#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "utils.h" 

struct clientsinfo *add_client(struct clientsinfo *head, int sockfd, char *name) {
	struct clientsinfo *new = malloc(sizeof (struct clientsinfo));
	struct clientsinfo *node = head;
	
	while (node->right != NULL) { 
		node = node->right;
	}
	
	node->right = new;

	new->fd = sockfd;
	new->name = name;
	new->left = node;
	new->right = NULL;
	
	return new;
}

void remove_client(struct clientsinfo *node) {
	if (node->right)
		node->right->left = node->left;	

	if (node->left)
		node->left->right = node->right;

	free(node);
}

struct addrinfo generate_socket_hints() {
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	return hints;
}

void get_address_info(struct addrinfo *hints, struct addrinfo **results) {
	int status;
	if ((status = getaddrinfo(NULL, "5001", hints, results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}
}

int create_socket(struct addrinfo *addr) {
	int sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (sockfd == -1) {
		perror("socket");	
		exit(1);
	}
	return sockfd;
}

void set_port_reuse(int fd) {
	int yes = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
		perror("setsockopt");
		exit(1);
	}
}

void bind_socket(int fd, struct addrinfo *addr) {
	int sockbind = bind(fd, addr->ai_addr, addr->ai_addrlen);
	if (sockbind == -1) {
		perror("bind");
		exit(1);
	}
}

void listen_socket(int fd, int connections) {
	int socklisten = listen(fd, connections);
	if (socklisten == -1) {
		perror("listen");
		exit(1);
	}
}

void connect_socket(int fd, struct addrinfo *results) {
	int res = connect(fd, results->ai_addr, results->ai_addrlen);

	if (res == -1) {
		perror("connect");
		exit(1);
	}
}
