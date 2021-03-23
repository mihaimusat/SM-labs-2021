CC=g++
CFLAGS=-Wall -lm

.PHONY: clean all

all: omega

omega: omega.cpp
	$(CC) $? -o $@ $(CFLAGS)

clean:
	rm -f omega
