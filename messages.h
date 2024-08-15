#ifndef MESSAGES_H
#define MESSAGES_H

#include <sys/ioctl.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

struct message {
    char *message;
    char *user;
    struct message *left, *right;
};

struct message *add_message(struct message *head, char *user, char *message);
struct message create_empty_message();

struct winsize get_window_size();

void clear_screen();
void display_title(struct winsize *w, char *title);
void display_user_input(struct winsize *w);
void display_messages(struct winsize *w, struct message *tail);
void handle_user_input(struct winsize *w, char *msg);

#endif // MESSAGES_H
