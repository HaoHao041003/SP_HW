CC = gcc
TARGET = isock_server isock_client

all: $(TARGET)

isock_server: isock_server.o lookup5.o
	$(CC) $^ -o $@

isock_client: main.o lookup8.o
	$(CC) $^ -o $@

%.o: dict.h

.PHONY: all clean

clean:
	rm -f $(TARGET) $(wildcard *.o)
	-killall isock_server