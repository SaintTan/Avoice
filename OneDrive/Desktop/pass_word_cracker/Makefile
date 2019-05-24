CC = gcc
CFLAGS= -std=c99 -O3 -Wall -Wpedantic
DEPS = shah256.h
OBJ = crack.o sha256.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

crack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
