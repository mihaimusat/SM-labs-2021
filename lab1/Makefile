CC=gcc
CFLAGS=-Wall -Werror -lm -fopenmp

.PHONY: clean all

all: crcw crew 

crcw: crcw.c
	$(CC) $? -o $@ $(CFLAGS)

crew: crew.c
	$(CC) $? -o $@ $(CFLAGS)

clean:
	rm -f crcw crew
