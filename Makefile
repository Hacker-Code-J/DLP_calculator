CC = gcc
CFLAGS = -Wall -Wextra -g -O2
OBJS = setup.o BigInteger.o operation.o main.o

# Targets
a.out: $(OBJS)
	$(CC) -o $@ $^

setup.o: setup.c
	$(CC) $(CFLAGS) -c -o $@ $<

BigInteger.o: BigInteger.c
	$(CC) $(CFLAGS) -c -o $@ $<

operation.o: operation.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	./a.out

debug:
	valgrind --leak-check=full ./a.out

clean:
	rm -f *.o a.out *.bin *.txt

clean-test:
	rm -f test.py test.txt speed.txt

test:
	./a.out > test.py
	mv test.py visualization
	cd visualization
	python3 test.py > test.txt
	cd ..

chart:
	./a.out > test.py
	mv test.py visualization
	cd visualization
	python3 test.py > test.txt
	python3 sr_chart.py
	cd ..

speed:
	./a.out > speed.txt
	mv speed.txt visualization
	cd visualization
	python3 time_compare_chart.py
	cd ..

speed2:
	./a.out > speed.txt
	mv speed.txt visualization
	cd visualization
	python3 time_compare_mul.py
	cd ..

.PHONY: clean