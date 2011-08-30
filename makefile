CXX = g++
CCFLAGS = -Wall

all: term-do

term-do: vt100.o matcher.o term-do.o
	$(CXX) vt100.o matcher.o term-do.o $(CCFLAGS) -o term-do

vt100.o: vt100.cpp vt100.h
	$(CXX) -c vt100.cpp $(CCFLAGS) -o vt100.o

matcher.o: matcher.cpp matcher.h
	$(CXX) -c matcher.cpp $(CCFLAGS) -o matcher.o

term-do.o: term-do.cpp term-do.h
	$(CXX) -c term-do.cpp $(CCFLAGS) -o term-do.o

clean:
	-rm term-do
	-rm *.o
	-rm *~