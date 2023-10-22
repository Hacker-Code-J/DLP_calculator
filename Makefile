CC = gcc
CFLAGS = -Wall -Wextra -g -O2
OBJS = main.o

# Targets
a.out: $(OBJS)
	$(CC) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o a.out

.PHONY: clean