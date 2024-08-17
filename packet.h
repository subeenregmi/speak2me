#ifndef PACKET_H
#define PACKET_H

#define PAYLOAD_SIZE 1024

enum Type {
    LIST = 0,
    MSG,
    COMMAND
};

struct Packet {
    enum Type t;
    char payload[PAYLOAD_SIZE];
};

#endif // PACKET_H
