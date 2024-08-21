#ifndef TUI_H
#define TUI_H

#include <sys/ioctl.h>

#include "messages.h"

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

struct winsize get_window_size();
void clear_screen();
void display_title(struct winsize *w, char *title);
void display_user_input(struct winsize *w, char *buffer);
void display_messages(struct winsize *w, struct message *tail);
void handle_user_input(struct winsize *w, char *msg);

#endif
