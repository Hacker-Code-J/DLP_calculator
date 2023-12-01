CC=gcc
CFLAGS=-I. -O2
OBJS=utils.o arithmetic.o
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

# Create static library
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Compile main.c to main.o
$(MAIN): main.c
	$(CC) -c -o $(MAIN) main.c $(CFLAGS)

# Link everything to create the executable
$(EXECUTABLE): $(LIB) $(MAIN)
	$(CC) -o $(EXECUTABLE) $(MAIN) -L. -larith

# Clean target
clean:
	rm -f $(OBJS) $(LIB) $(MAIN) $(EXECUTABLE)