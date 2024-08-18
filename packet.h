#ifndef PACKET_H
#define PACKET_H

#define PAYLOAD_SIZE 1024

enum Type {
    MSG = 0,
    COMMAND = 1,
    CONN = 2
};

struct message_payload {
    char channel[64];
    char msg[956];
};

struct command_payload {
    char command[1020];
};

struct connection_payload {
    char channel[64];
    char public_key[956];
};

struct packet {
    char sequence;
    char type;
    char size[2];
    union {
        struct message_payload msg_p;
        struct command_payload cmd_p;
        struct connection_payload conn_p;
    };
};

#endif // PACKET_H
