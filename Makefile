CC = gcc
CFLAGS = -Wall -Wextra -g -O2
LIBS = -lflint -lgmp
OBJS = setup.o BigInteger.o operation.o main.o

# Targets
a.out: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

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

leak:
	valgrind --leak-check=full --show-leak-kinds=all ./a.out

clean:
	@echo "Cleaning up..."
	rm -f *.o a.out *.bin *.txt
	@echo "Cleaned."

DIR=Views
FILES_TO_CLEAN=$(DIR)/test.py $(DIR)/test.txt $(DIR)/speed.txt $(DIR)/lib_test.txt
clean-test:
	@echo "Cleaning up..."
	-rm -f test.py test.txt speed.txt
	-rm -f $(FILES_TO_CLEAN)
	@echo "Cleaned."

convert:
	@echo "Connecting converter..."
	(cd Tools && python3 converter.py)
	@echo "Disconnect Converter."

sage:
	@echo "Loading SageMath..."
	(cd Tests-SageMath && sage)
	@echo "Quit SageMath."

test:
	@echo "Creating text.txt ..."
	./a.out > test.py
	python3 test.py > test.txt
	mv test.py test.txt Views/
	@echo "Created."

chart:
	@echo "Visualizing ..."
	./a.out > test.py
	python3 test.py > test.txt
	mv test.py test.txt Views/
	(cd Views && python3 success_chart2.py)
	@echo "Completed."

speed:
	@echo "Visualizing ..."
	./a.out > speed.txt
	@echo "Converting to Text File ..."
	mv speed.txt Views/
	(cd Views && python3 time_compare_chart.py)
	@echo "Completed."

speed-mul:
	@echo "Visualizing ..."
	./a.out > speed.txt
	@echo "Converting to Text File ..."
	mv speed.txt Views/
	(cd Views && python3 time_compare_mul.py)
	@echo "Completed."

speed-len:
	@echo "Visualizing ..."
	./a.out > speed.txt
	@echo "Converting to Text File ..."
	mv speed.txt Views/
	(cd Views && python3 time_compare_mul.py)
	@echo "Completed."

flag:
	@echo "Visualizing ..."
	(cd Views && python3 Krtsb_FLAG.py)
	@echo "Completed."

lib:
	@echo "Converting to Text File ..."
	./a.out > lib_test.txt
	mv lib_test.txt Views/
	(cd Views && python3 lib_test.py)
	@echo "Completed."

.PHONY: clean