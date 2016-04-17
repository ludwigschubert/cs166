CFLAGS = -Wall -Wpedantic -std=gnu99 -O0 -g
CC = gcc

OBJECTS = main.o kmp.o

default: run-tests

run-tests: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c kmp.h

kmp.o: kmp.c kmp.h

clean:
	rm -f run-tests *.o *~
