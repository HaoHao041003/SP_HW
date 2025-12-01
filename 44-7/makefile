CC = gcc
SRCS   = $(wildcard *.c)
TARGET = $(SRCS:.c=)

all: $(TARGET)
$(TARGET): %: %.o
	$(CC) $^ -o $@

.PHONY: all clean

clean:
	rm -f $(TARGET) $(wildcard *.o)