CXX = g++
CCFLAGS = -Wall -O3
LDFLAGS = -ldl -lrt
SOURCES = $(wildcard *.cpp)
REUSED_OBJECTS = $(wildcard ../*.o)

all: client server

client: client.o $(REUSED_OBJECTS)
	$(CXX) -I.. $(LDFLAGS) client.o $(OBJECTS) $(REUSED_OBJECTS) $(CCFLAGS) -o $@

server: server.o $(REUSED_OBJECTS)
	$(CXX) -I.. $(LDFLAGS) server.o $(OBJECTS) $(REUSED_OBJECTS) $(CCFLAGS) -o $@

%.o: %.cpp
	$(CXX) -I.. -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~