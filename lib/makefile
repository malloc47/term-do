EXEC = file.so
CXX = g++
CCFLAGS = -Wall -fPIC
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
