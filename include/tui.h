#ifndef TUI_H
#define TUI_H

#include <sys/ioctl.h>

#define GOTOXY(x,y) printf("\033[%d;%dH", (y), (x))

#define HORIZONTAL_BORDER   '-'
#define VERTICAL_BORDER     '|'
#define CORNER_BORDER       '+'
#define INPUT_CHARACTER     '>'

#define MAX_CONTAINER_COMPONENTS 64

enum c_type {
    container = 0,
    text      = 1,
    button    = 2,
    input_box = 3,
};

enum c_options {
    disabled  = 0b00001,
    hover     = 0b00010,
    scroll    = 0b00100,
    expand    = 0b01000,
    hidden    = 0b10000,
};

struct c_position {
    int s_x;
    int s_y;
    int e_x;
    int e_y;
};

struct c_text {
    char *msg;
};

struct c_button {
    char *msg;
    void (*on_submit)();
};

struct c_input_box {
    char *msg;
    char *output;
    void (*on_submit)(char *);
};

struct component {
    enum c_type type;
    struct c_position pos;
    unsigned int opt;
    union {
        struct c_text text;
        struct c_button button;
        struct c_input_box input;
        struct component *container[MAX_CONTAINER_COMPONENTS];
    };
};

struct winsize get_window_size();
void clear_screen();

void draw_component(struct component *c);

#endif
