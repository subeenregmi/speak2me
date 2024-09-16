#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <stdlib.h>
#include <termio.h>

#include "utils.h"
#include "tui.h"
#include "messages.h"

thrd_t listener;
mtx_t display_mtx;
struct message msgs;
struct message *tail;
char *input_buffer;

struct message *get_tail(struct message *m) {
	if (m == NULL) {
		return NULL;
	}	
	struct message *tail = m;
	while (tail->right != NULL)
		tail = tail->right;

	return tail;
}

void display_ui(struct winsize w) {
	clear_screen();
	display_title(&w, "Subeen's Chatroom!");
	display_messages(&w, tail);
	display_user_input(&w, input_buffer);
	gotoxy(5 + (int)strlen(input_buffer), w.ws_row-1);
	/*
	if (strlen(buffer) > 0) {
		printf("%s", buffer);
		gotoxy(5, w.ws_row-1 + (int)strlen(buffer));
	}
	*/
	fflush(stdout);
}

int listen_to_messages(void *arg) {
	int sockfd = *(int *)arg;
	while (1) {
		struct winsize w = get_window_size();
		char *buffer = malloc(sizeof(char)*1024);
		int bytes = recv(sockfd, buffer, 1024, 0);
		if (bytes == 0) {
			printf("Server has disconnected, bye!\n");
			exit(1);
		}
		char *msg = malloc(sizeof(char) * strlen(buffer));
		strcpy(msg, buffer);
		free(buffer);
		tail = add_message(&msgs, "Other", msg);
		display_ui(w);
	}
	return 1;
}

void handle_client(int fd) {
	while (1) {
		struct winsize w = get_window_size();
		display_ui(w);

		handle_user_input(&w, input_buffer);
		char *msg = malloc(sizeof(char) * strlen(input_buffer));
		strcpy(msg, input_buffer);
		strcpy(input_buffer, "");
		
		if (*msg) {
			tail = add_message(&msgs, "Me", msg);
			send(fd, msg, strlen(msg), 0);
		}
	}
}


int main(void) {

	static struct termios oldt, newt;

	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);          
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	input_buffer = calloc(1024, sizeof(char));
	
	struct addrinfo hints = generate_socket_hints();
	struct addrinfo *results;

	get_address_info(&hints, &results);
	int sockfd = create_socket(results);
	connect_socket(sockfd, results);

	thrd_create(&listener, listen_to_messages, &sockfd);
	thrd_detach(listener);
	
	handle_client(sockfd);
}
