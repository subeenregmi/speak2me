#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/ioctl.h>

#include "messages.h"

#define HOR_BORDER '-'
#define VERT_BORDER '|'
#define INPUT_CHAR '>'

void clear_screen(){
	printf("\e[1;1H\e[2J");
}

struct winsize get_window_size(){
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	return w;
}

void display_title(struct winsize *w, char *title){

	for(int i=0; i<w->ws_col; i++){
		printf("%c", HOR_BORDER);
	}
	printf("\n");

	int padding = (w->ws_col - strlen(title))/2;
	printf("%c", VERT_BORDER);
	for(int i=0; i<padding-1; i++){
		printf(" ");
	}
	printf("%s", title);
	for(int i=0; i<padding; i++){
		printf(" ");
	}
	printf("%c\n", VERT_BORDER);

	for(int i=0; i<w->ws_col; i++){
		printf("%c", HOR_BORDER);
	}
	printf("\n");
}

void display_user_input(struct winsize *w) {
	for(int i=0; i<w->ws_col; i++){
		printf("%c", HOR_BORDER);
	}
	printf("\n");

	printf("%c", VERT_BORDER);

	printf(" %c", INPUT_CHAR);

	for(int i=0; i<w->ws_col-4; i++){
		printf(" ");
	}

	printf("%c", VERT_BORDER);

	for(int i=0; i<w->ws_col; i++){
		printf("%c", HOR_BORDER);
	}
}

void display_messages(struct winsize *w, struct message *msg_tail){
	int available_lines= w->ws_row - 6;
	struct message *node = msg_tail;
	
	while(available_lines > 0){
		char *message;
		sprintf(message, "%s: %s\n", node->user, node->message);
		int lines_to_take = (strlen(message) / w->ws_col) + 1;

		if((available_lines - lines_to_take) <= 0){
			break;
		}
		if(node->left == NULL){
			break;
		}
		available_lines -= lines_to_take;
		node = node->left;
	
	}
	while(node != NULL){
		printf("%s: %s\n", node->user, node->message);
		node = node->right;

	}
	for(int i=0; i<available_lines-1; i++){
		printf("\n");
	}

}

void handle_user_input(struct winsize *w, char *msg){
	int user_x = 5;
	int user_y = w->ws_row - 1;
	gotoxy(user_x, user_y);

	fgets(msg, 1024, stdin);
	msg[strlen(msg)-1] = '\0';
}

void start_message_displayer(struct message *msgs){
	clear_screen();
	struct message *tail = msgs;

	while(tail->right != NULL){
		tail = tail->right;	
	}

	while(1){
		struct winsize w = get_window_size();
		char *msg = malloc(sizeof(char)*1024);
		clear_screen();
		display_title(&w, "Subeen's Chatroom!");
		display_messages(&w, tail);
		display_user_input(&w);
		handle_user_input(&w, msg);
		tail = add_message(msgs, "Subeen", msg);
	}
}
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
