#include <stdio.h>
#include <string.h>

#include "packet.h"

void hex_format(char *dst, int src, unsigned int length) {
	sprintf(dst, "%0*x", length, src);
}

int validate_msg(struct message_payload msg) {
	if (strlen(msg.channel) > CHANNEL_SIZE) {
		return 1;
	}

	if (strlen(msg.from) > FROM_SIZE) {
		return 1;
	}

	if (strlen(msg.to) > TO_SIZE) {
		return 1;
	}

	if (strlen(msg.msg) > MSG_SIZE) {
		return 1;
	}

	return 0;
}

char *serialize_packet_type(char *buffer, enum Type type) {
	hex_format(buffer, type, 2);
	return buffer+2;
}

char *serialize_data(char *buffer, char *data, unsigned int size_bytes) {
	int size = strlen(data);
	hex_format(buffer, size, size_bytes);

	buffer += size_bytes;

	for (int i = 0; i < size; i++) {
		*buffer++ = data[i];
	}

	return buffer;
}

void serialize_message_payload(char *buffer, struct message_payload msg) {
	buffer = serialize_data(buffer, msg.channel, 2);
	buffer = serialize_data(buffer, msg.from, 2);
	buffer = serialize_data(buffer, msg.to, 2);
	buffer = serialize_data(buffer, msg.msg, 4);
}

int serialize_packet(struct packet *p, char *buffer) {
	buffer = serialize_packet_type(buffer, p->type);

	switch (p->type) {
		case MSG:
			if (validate_msg(p->msg_p)) {
				return 1;
			}
			serialize_message_payload(buffer, p->msg_p);
			break;
		case COMMAND:
			break;
		case CONN:
			break;
	}
	return 0;
}

int main() {
	enum Type t = MSG;
	struct message_payload msg = {
		.channel = "Chatroom",
		.from = "Subeen",
		.to = "John",
		.msg = "Hello everyone!",
	};

	struct packet p = {
		.type = t,
		.msg_p = msg,
	};

	printf("type: %d, msg-channel: %s, from: %s, to: %s, msg: %s\n", 
			p.type, p.msg_p.channel, p.msg_p.from, p.msg_p.to, p.msg_p.msg);

	char serialized[1024] = "";

	int ret = serialize_packet(&p, serialized);
	printf("return: %d\nserialized: %s\n", ret, serialized);
}
