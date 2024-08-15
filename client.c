#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"
#include "messages.h"

thrd_t listener;
struct message msgs;
struct message *tail;

struct message *get_tail(struct message *m) {
	if (m == NULL) {
		return NULL;
	}	
	struct message *tail = m;
	while (tail->right != NULL)
		tail = tail->right;

	return tail;
}

int listen_to_messages(void *arg) {
	int sockfd = *(int *)arg;
	while (1) {
		struct winsize w = get_window_size();
		char *msg = malloc(sizeof(char)*1024);
		int bytes = recv(sockfd, msg, 1024, 0);
		if (bytes == 0) {
			printf("Server has disconnected, bye!\n");
			exit(1);
		}
		tail = add_message(&msgs, "Other", msg);
		clear_screen();
		display_title(&w, "Subeen's Chatroom!");
		display_messages(&w, tail);
		display_user_input(&w);
	}

	return 1;
}

void handle_client(int fd) {
	while (1) {
		struct winsize w = get_window_size();
		clear_screen();
		display_title(&w, "Subeen's Chatroom!");
		display_messages(&w, tail);
		display_user_input(&w);

		char *msg = malloc(sizeof(char)*1024);
		handle_user_input(&w, msg);

		if (*msg) {
			tail = add_message(&msgs, "Me", msg);
			send(fd, msg, strlen(msg), 0);
		}
	}
}


int main(void) {
	struct addrinfo hints = generate_socket_hints();
	struct addrinfo *results;

	get_address_info(&hints, &results);
	int sockfd = create_socket(results);
	connect_socket(sockfd, results);

	thrd_create(&listener, listen_to_messages, &sockfd);
	thrd_detach(listener);
	
	handle_client(sockfd);
}
