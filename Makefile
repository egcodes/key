CC = gcc
CFLAGS = -g -Wall

TARGETS = key

all: clean
  $(CC) $(CFLAGS) key.c -o $(TARGETS)

install:
	cp $(TARGETS) /bin

clean:
	rm -f /bin/$(TARGETS)
