CXX = g++
CCFLAGS = -Wall -fPIC # -g -ggdb
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = $(SOURCES:.cpp=.so)

all: $(EXEC)

%.so: %.o
	$(CXX) $< $(CCFLAGS) -shared -Wl,-soname,$@ -o $@

%.o: %.cpp
	$(CXX) -c $(CCFLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~
