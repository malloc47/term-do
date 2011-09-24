CXX = g++
CCFLAGS = -Wall -fPIC -O3 # -g -ggdb
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = $(SOURCES:.cpp=.so)

all: $(EXEC)

%.so: %.o
	$(CXX) $< tst.o $(CCFLAGS) -shared -Wl,-soname,$@ -o $@

%.o: %.cpp
	$(CXX) -c $(CCFLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS) tst.o
	-rm *~

bootstrap:
	cd ../ ; $(CXX) -c $(CCFLAGS) tst.cpp -o lib/tst.o
