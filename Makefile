CC = gcc
CFLAGS = -g3 --std=gnu99

smallsh: smallsh.c
	$(CC) $(CFLAGS) -o smallsh smallsh.c

clean:
	rm -f smallsh

.PHONY:
	clean
