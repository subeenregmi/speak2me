#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/ioctl.h>

#include "messages.h"

#define HOR_BORDER '-'
#define VERT_BORDER '|'
#define INPUT_CHAR '>'

struct message *add_message(struct message *head, char *user, char *message){
	struct message *new = malloc(sizeof (struct message));
	if (head == NULL) {
		new->message = message;
		new->user = user;
		new->right = NULL;
		return new;
	}

	struct message *node = head;

	while (node->right != NULL) {
		node = node->right;
	}

	node->right = new;
	
	new->left = node;
	new->message = message;
	new->user = user;
	new->right = NULL;
	
	return new;
}

struct message create_empty_message() {
	struct message msg;
	msg.message = NULL;
	msg.user = NULL;
	msg.left = NULL;
	msg.right = NULL;
	return msg;
}
