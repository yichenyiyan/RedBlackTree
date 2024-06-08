CXX=g++
CXXFLAGS=-std=c++11 -I../include
LDFLAGS=

all: main

main: ../src/RedBlackTree.cc main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f main

