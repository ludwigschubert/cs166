CXXFLAGS = -std=c++11 -Wall -Werror -O0 -g
CXX = g++

OBJECTS = Main.o Hashes.o ChainedHashTable.o SecondChoiceHashTable.o LinearProbingHashTable.o RobinHoodHashTable.o CuckooHashTable.o

default: run-tests

run-tests: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Main.o: Main.cc Timing.h Hashes.h ChainedHashTable.h SecondChoiceHashTable.h LinearProbingHashTable.h RobinHoodHashTable.h CuckooHashTable.h

%.o: %.cc %.h Hashes.h

clean:
	rm -f run-tests *.o *~
