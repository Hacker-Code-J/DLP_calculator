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

DIR=visualization
FILES_TO_CLEAN=$(DIR)/test.py $(DIR)/test.txt $(DIR)/speed.txt
clean-test:
	@echo "Cleaning up..."
	-rm -f test.py test.txt speed.txt
	-rm -f $(FILES_TO_CLEAN)
	@echo "Cleaned."

test:
	@echo "Creating text.txt ..."
	./a.out > test.py
	python3 test.py > test.txt
	mv test.py test.txt visualization/
	@echo "Created."

chart:
	@echo "Visualizing ..."
	./a.out > test.py
	python3 test.py > test.txt
	mv test.py test.txt visualization/
	(cd visualization && python3 sr_chart.py)
	@echo "Visualized."

speed:
	@echo "Visualizing ..."
	./a.out > speed.txt
	mv speed.txt visualization/
	(cd visualization && python3 time_compare_chart.py)
	@echo "Visualized."

speed2:
	@echo "Visualizing ..."
	./a.out > speed.txt
	mv speed.txt visualization/
	(cd visualization && python3 time_compare_mul.py)
	@echo "Visualized."

.PHONY: clean