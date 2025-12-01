CC = gcc
TARGET = seqnum_server seqnum_client
OBJS = get_num.o error_functions.o

all: $(TARGET)

seqnum_server: seqnum_server.o $(OBJS)
	$(CC) $^ -o $@

seqnum_client: seqnum_client.o $(OBJS)
	$(CC) $^ -o $@

seqnum_server.o: seqnum_server.c seqnum.h tlpi_hdr.h
get_num.o: get_num.c get_num.h
error_functions.o: error_functions.c error_functions.h tlpi_hdr.h ename.c.inc

.PHONY: all clean

clean:
	rm -f $(TARGET) $(wildcard *.o)