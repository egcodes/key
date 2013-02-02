CC = gcc
CFLAGS = -g -Wall

TARGETS = key

all:
	$(CC) $(CFLAGS) key.c -o $(TARGETS)

install:
	cp $(TARGETS) /bin

clean:
	rm -f /bin/$(TARGETS)
