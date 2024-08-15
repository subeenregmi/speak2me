#ifndef UTILS_H 
#define UTILS_H

#include <netdb.h>

struct clientsinfo {
    int fd;
    char *name;
    struct clientsinfo *left, *right;
};

struct clientsinfo *add_client(struct clientsinfo *head, int sockfd, char *name);
void remove_client(struct clientsinfo *node);

struct addrinfo generate_socket_hints();
void get_address_info(struct addrinfo *hints, struct addrinfo **results);
int create_socket(struct addrinfo *addr);
void set_port_reuse(int fd);
void bind_socket(int fd, struct addrinfo *addr);
void listen_socket(int fd, int connections);
void connect_socket(int fd, struct addrinfo *results);

#endif
