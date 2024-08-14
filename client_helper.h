#ifndef CLIENTS_HELPER_H
#define CLIENTS_HELPER_H

struct clientsinfo {
    int fd;
    char *name;
    struct clientsinfo *left, *right;
};

struct clientsinfo *add_client(struct clientsinfo *head, int sockfd, char *name);
void remove_client(struct clientsinfo *node);

#endif
