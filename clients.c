#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include "clients.h" 

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

