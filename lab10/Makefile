CC=g++
CFLAGS=-Wall -lm

.PHONY: clean all

all: mesi

mesi: mesi.cpp
	$(CC) $? -o $@ $(CFLAGS)

clean:
	rm -rf mesi
