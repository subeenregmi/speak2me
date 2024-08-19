#include <stdlib.h>
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
	return buffer + 2;
}

char *serialize_data(char *buffer, char *data, unsigned int size_b) {
	int size = strlen(data);
	hex_format(buffer, size, size_b);

	buffer += size_b;

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

char *deserialize_packet_type(struct packet *dst, char *src) {
	char type_hex[2];
	memcpy(type_hex, src, 2);

	enum Type t = (enum Type) strtol(type_hex, NULL, 16);
	dst->type = t;

	return src + 2;
}

char *deserialize_data(char *dst, char *src, unsigned int size_b) {
	char size_hex[size_b];
	memcpy(size_hex, src, size_b);

	int size = (int) strtol(size_hex, NULL, 16);
	src += size_b;
	memcpy(dst, src, size);

	return src + size;
}

void deserialize_message_payload(struct packet *dst, char *src) {
	src = deserialize_data(dst->msg_p.channel, src, 2);
	src = deserialize_data(dst->msg_p.from, src, 2);
	src = deserialize_data(dst->msg_p.to, src, 2);
	src = deserialize_data(dst->msg_p.msg, src, 4);
}

int deserialize_packet(struct packet *dst, char *src) {
	src = deserialize_packet_type(dst, src);

	switch (dst->type) {
		case MSG:
			deserialize_message_payload(dst, src);
			return validate_msg(dst->msg_p);
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


	char channel[100] = "";
	char from[100] = "";
	char to[100] = "";
	char msgp[100] = "";

	struct message_payload post_msg = {
		.channel = channel,
		.from = from,
		.to = to,
		.msg = msgp,
	};
	struct packet post = {
		.type = 99,
		.msg_p = post_msg,
	};
	ret = deserialize_packet(&post, serialized);


	printf("type: %d, msg-channel: %s, from: %s, to: %s, msg: %s\n", 
			post.type, post.msg_p.channel, post.msg_p.from, post.msg_p.to, 
			post.msg_p.msg);
	printf("return: %d\n", ret);
}
