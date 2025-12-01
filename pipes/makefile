CC = gcc
TARGET = fifo_seqnum_server fifo_seqnum_client
OBJS = get_num.o error_functions.o

all: $(TARGET)

$(TARGET): fifo_seqnum_server.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

fifo_seqnum_server.o: fifo_seqnum_server.c fifo_seqnum.h tlpi_hdr.h
get_num.o: get_num.c get_num.h
error_functions.o: error_functions.c error_functions.h tlpi_hdr.h ename.c.inc

.PHONY: all clean

clean:
	rm -f $(TARGET) $(OBJS) fifo_seqnum_server.o