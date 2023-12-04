CC=gcc
CFLAGS=-Wall -Wextra -g -I. -ITests -O2
OBJS=utils.o arithmetic.o measure.o
LIB=libarith.a
MAIN=main.o
EXECUTABLE=app

# Default target
all: $(EXECUTABLE)

# Compile utils.c to utils.o
utils.o: utils.c utils.h config.h
	$(CC) -c -o utils.o utils.c $(CFLAGS)

# Compile arithmetic.c to arithmetic.o
arithmetic.o: arithmetic.c arithmetic.h utils.h config.h
	$(CC) -c -o arithmetic.o arithmetic.c $(CFLAGS)

measure.o: Tests/measure.c Tests/measure.h
	$(CC) -c -o measure.o Tests/measure.c $(CFLAGS)

# Create static library
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Compile main.c to main.o
$(MAIN): main.c
	$(CC) -c -o $(MAIN) main.c $(CFLAGS)

# Link everything to create the executable
$(EXECUTABLE): $(LIB) $(MAIN)
	$(CC) -o $(EXECUTABLE) $(MAIN) -L. -larith -lm

# Clean target
DIR=Views
FILES_TO_CLEAN=$(DIR)/test.py $(DIR)/test.txt $(DIR)/speed.txt
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(LIB) $(MAIN) $(EXECUTABLE)
	rm -f $(FILES_TO_CLEAN)
	@echo "Cleaned."

# Check Memory Leak
leak:
	valgrind --leak-check=full --show-leak-kinds=all ./app

chart:
	@echo "Visualizing ..."
	./app > test.py
	python3 test.py > test.txt
	mv test.py test.txt Views/
	(cd Views && python3 success_chart.py)
	@echo "Completed."

speed:
	@echo "Visualizing ..."
	./app > speed.txt
	mv speed.txt Views/
	(cd Views && python3 compare_chart.py)
	@echo "Completed."