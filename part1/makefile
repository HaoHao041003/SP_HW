CC = gcc
TARGET = usock_server usock_client

all: $(TARGET)

usock_server: usock_server.o lookup5.o
	$(CC) $^ -o $@

usock_client: main.o lookup7.o
	$(CC) $^ -o $@

%.o: dict.h

.PHONY: all clean

clean:
	rm -f $(TARGET) $(wildcard *.o) TUNNEL
	-killall usock_server