CC=g++
CFLAGS=-Wall -lm

.PHONY: clean all

all: benes

benes: benes.cpp
	$(CC) $? -o $@ $(CFLAGS)

clean:
	rm -f benes
