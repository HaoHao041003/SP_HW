CC = gcc
TARGET = udp_server udp_client

all: $(TARGET)

udp_server: udp_server.o lookup5.o
	$(CC) $^ -o $@

udp_client: main.o lookup9.o
	$(CC) $^ -o $@

%.o: dict.h

.PHONY: all clean

clean:
	rm -f $(TARGET) $(wildcard *.o)
	-killall udp_server