CC = gcc
CFLAGS = -Wall -Wextra -g -O2
OBJS = setup.o BigInteger.o main.o

# Targets
a.out: $(OBJS)
	$(CC) -o $@ $^

setup.o: setup.c
	$(CC) $(CFLAGS) -c -o $@ $<

BigInteger.o: BigInteger.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o a.out

.PHONY: clean