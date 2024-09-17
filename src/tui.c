#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/ioctl.h>

#include "tui.h"

void clear_screen(){
	printf("\e[1;1H\e[2J");
}

struct winsize get_window_size(){
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	return w;
}

void draw_component(struct component *c) {
	
}

int main(void) {
	struct winsize w = get_window_size();
	struct component screen = {
		.type = container,
		.pos = {
			.s_x = 0,
			.s_y = 0,
			.e_x = w.ws_col - 1,
			.e_y = w.ws_row - 1
		},
		.opt = 0,
		.container = {NULL} 
	};
	clear_screen();
	draw_component(&screen);
}
