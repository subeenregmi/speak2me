CC=gcc
common_objs = bin/messages.o bin/packet.o bin/tui.o bin/utils.o

all: client server

client: bin/client.o ${common_objs}
	gcc -o bin/client $^ -Iinclude

server: bin/server.o ${common_objs}
	gcc -o bin/server $^ -Iinclude

bin/%.o: src/%.c
	gcc -o $@ -c $< -Iinclude

clean:
	rm bin/*

bin/%: bin/%.o ${common_objs}
	gcc -o $@ $^ -Iinclude
