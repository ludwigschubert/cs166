CXXFLAGS = -std=c++11 -Wall -Werror -O0 -g
CXX = g++

OBJECTS = Main.o SplayTree.o WeightBalancedTree.o StdSetTree.o Timing.o PerfectlyBalancedTree.o HashTable.o

default: run-tests

run-tests: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Main.o: Main.cc Timing.h StdSetTree.h SplayTree.h WeightBalancedTree.h

PerfectlyBalancedTree.o: PerfectlyBalancedTree.cc PerfectlyBalancedTree.h

SplayTree.o: SplayTree.cc SplayTree.h

StdSetTree.o: StdSetTree.cc StdSetTree.h

HashTable.o: HashTable.cc HashTable.h

Timing.o: Timing.cc Timing.h

WeightBalancedTree.o: WeightBalancedTree.cc WeightBalancedTree.h

clean:
	rm -f run-tests *.o *~
