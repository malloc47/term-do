EXEC = launcher.so
CXX = g++
CCFLAGS = -Wall -fPIC -g -ggdb
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

%.so: %.o
	$(CXX) $(OBJECTS) $(CCFLAGS) -shared -Wl,-soname,$@ -o $@

%.o: %.cpp
	$(CXX) -c $(CCFLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~
