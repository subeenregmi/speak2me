#ifndef MESSAGES_H
#define MESSAGES_H

struct message {
    char *message;
    char *user;
    struct message *left, *right;
} __attribute__((__packed__));

struct message *add_message(struct message *head, char *user, char *message);
struct message create_empty_message();

#endif // MESSAGES_H
