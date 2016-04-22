CFLAGS = -Wall -Wpedantic -std=gnu99 -O0 -g
CC = gcc

OBJECTS = main.o fenwick-tree.o

default: run-tests

run-tests: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c fenwick-tree.h

fenwick-tree.o: fenwick-tree.c fenwick-tree.h

clean:
	rm -f run-tests *.o *~
