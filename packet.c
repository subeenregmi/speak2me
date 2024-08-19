#include <stdio.h>
#include <string.h>

#include "packet.h"

void hex_format(char *dst, int src, unsigned int length) {
	sprintf(dst, "%0*x", length, src);
}

void serialize_packet_type(char *buffer, enum Type type) {
	hex_format(buffer, type, 2);
}

void serialize_message_payload(struct message_payload msg, char *buffer) {
	int channel_len = strlen(msg.channel);
	hex_format(buffer, channel_len, 1);
}

int serialize_packet(struct packet *p, char *buffer) {
	serialize_packet_type(buffer, p->type);
	int ret;
	switch (p->type) {
		case MSG:
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
		.to = "",
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
