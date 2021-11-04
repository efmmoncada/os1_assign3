CC = gcc
CFLAGS = -g3 --std=gnu99 -Wall

smallsh: smallsh.c smallsh.h
	$(CC) $(CFLAGS) -o smallsh smallsh.c

clean:
	rm -f smallsh

.PHONY:
	clean
