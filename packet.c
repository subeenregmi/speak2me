#include <stdio.h>
#include <string.h>

#include "packet.h"


void hex_format(char *dst, int src, unsigned int length) {
	char format[10] = "%0";
	char len[10];

	sprintf(len, "%dx", length);
	strcat(format, len);
	sprintf(dst, format, src);
}

int serialize_message_payload(struct message_payload msg, char *buffer) {
	int ch_len = strlen(msg.channel);

	char ch_size[2];
	sprintf(ch_size, "%01x", ch_len);

	buffer[0] = ch_size[0];

	return 0;
}

int serialize_packet(struct packet *p, char *buffer) {
	buffer[0] = (char) p->type + 48;

	int ret;
	switch (p->type) {
		case MSG:
			ret = serialize_message_payload(p->msg_p, buffer+2);
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

	// int ret = serialize_packet(&p, serialized);
	// printf("return: %d\nserialized: %s\n", ret, serialized);
	char dst[100] = "";
	hex_format(dst, 1331132113, 11);
	printf("formatted: %s\n", dst);
}
