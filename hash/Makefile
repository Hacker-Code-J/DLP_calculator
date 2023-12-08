# Makefile for compiling the hash table example

CC = gcc
CFLAGS = -Wall -g
TARGET = hashtb
OBJS = main.o hash.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c hash.h
	$(CC) $(CFLAGS) -c main.c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c

clean:
	rm -f $(TARGET) $(OBJS)

rebuild: clean all

.PHONY: all clean
