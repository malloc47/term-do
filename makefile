CXX = g++
CCFLAGS = -Wall

all: term-do

term-do: vt100.o matcher.o term-do.o
	$(CXX) vt100.o matcher.o term-do.o $(CCFLAGS) -o term-do

vt100.o: vt100.cpp
	$(CXX) -c vt100.cpp $(CCFLAGS) -o vt100.o

matcher.o: matcher.cpp
	$(CXX) -c matcher.cpp $(CCFLAGS) -o matcher.o

term-do.o: term-do.cpp
	$(CXX) -c term-do.cpp $(CCFLAGS) -o term-do.o

clean:
	-rm term-do
	-rm *.o
	-rm *~