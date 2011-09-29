CXX = g++
CCFLAGS = -Wall -fPIC -O3 # -g -ggdb
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = $(SOURCES:.cpp=.so)

all: $(EXEC)

%.so: %.o tst.o
	$(CXX) $< tst.o $(CCFLAGS) -shared -Wl,-soname,$@ -o $@

tst.o: ../tst.cpp ../tst.h
	$(CXX) -c $(CCFLAGS) ../tst.cpp -o tst.o

%.o: %.cpp
	$(CXX) -c $(CCFLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS) tst.o
	-rm *~