#ifndef PACKET_H
#define PACKET_H

#define CHANNEL_SIZE    256
#define FROM_SIZE       256
#define TO_SIZE         256
#define MSG_SIZE       8192 

enum Type {
    MSG = 0,
    COMMAND = 1,
    CONN = 2
};

struct message_payload {
    char *channel;
    char *from;
    char *to;
    char *msg;
};

struct packet {
    enum Type type;
    union {
        struct message_payload msg_p;
    };
};

void hex_format(char *dst, int src, unsigned int length);
int validate_msg(struct message_payload msg);

char *serialize_packet_type(char *buffer, enum Type type);
char *serialize_data(char *buffer, char *data, unsigned int size_bytes);

void serialize_message_payload(char *buffer, struct message_payload msg);
int serialize_packet(struct packet *p, char *buffer);


#endif // PACKET_H
