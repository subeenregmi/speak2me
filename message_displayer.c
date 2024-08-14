#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/ioctl.h>

#include "messages.h"

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

char horizontal_border = '-';
char vertical_border = '|';
char input_char = '>';

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
		printf("%c", horizontal_border);
	}
	printf("\n");

	int padding = (w->ws_col - strlen(title))/2;
	printf("%c", vertical_border);
	for(int i=0; i<padding-1; i++){
		printf(" ");
	}
	printf("%s", title);
	for(int i=0; i<padding; i++){
		printf(" ");
	}
	printf("%c\n", vertical_border);

	for(int i=0; i<w->ws_col; i++){
		printf("%c", horizontal_border);
	}
	printf("\n");
}

void display_user_input(struct winsize *w) {
	for(int i=0; i<w->ws_col; i++){
		printf("%c", horizontal_border);
	}
	printf("\n");

	printf("%c", vertical_border);

	printf(" %c", input_char);

	for(int i=0; i<w->ws_col-4; i++){
		printf(" ");
	}

	printf("%c", vertical_border);

	for(int i=0; i<w->ws_col; i++){
		printf("%c", horizontal_border);
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

int main(void){
	clear_screen();
	clear_screen();


	struct message *msgs = add_message(NULL, "John", "Hello anyone online?");
	add_message(msgs, "Alice", "Hi John, I'm here!");
	add_message(msgs, "Bob", "Hey John, how's it going?");
	add_message(msgs, "John", "Hey Alice, Bob! I'm doing well. Just checking in.");
	add_message(msgs, "Alice", "Nice to hear that! What's up?");
	add_message(msgs, "John", "Not much, just wanted to see if anyone was around.");
	add_message(msgs, "Bob", "We're always here for you, John!");
	add_message(msgs, "Alice", "Absolutely! Anything you need?");
	struct message *tail = add_message(msgs, "John", "Thanks, you two! Just wanted to chat.");

	while(1){
		struct winsize w = get_window_size();
		char *msg = malloc(sizeof(char)*1024);
		display_title(&w, "Subeen's Chatroom!");
		display_messages(&w, tail);
		display_user_input(&w);
		handle_user_input(&w, msg);
		tail = add_message(msgs, "Subeen", msg);
	}
}
