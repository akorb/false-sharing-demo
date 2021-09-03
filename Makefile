CC=gcc
CFLAGS=-Wall -fopenmp -O3

all: padded unpadded

padded:
	$(CC) $(CFLAGS) -D PADDED -o false-sharing-padded false-sharing.c

unpadded:
	$(CC) $(CFLAGS) -o false-sharing-unpadded false-sharing.c

clean:
	rm false-sharing-padded false-sharing-unpadded
