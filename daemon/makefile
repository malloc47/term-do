EXEC = client
CXX = g++
CCFLAGS = -Wall -O3
LDFLAGS = -ldl -lrt
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
REUSED_OBJECTS = $(wildcard ../*.o)

all: $(EXEC)

$(EXEC): $(OBJECTS) $(REUSED_OBJECTS)
	$(CXX) -I.. $(LDFLAGS) $(OBJECTS) $(REUSED_OBJECTS) $(CCFLAGS) -o $@

%.o: %.cpp
	$(CXX) -I.. -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~