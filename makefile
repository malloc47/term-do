EXEC = term-do
CXX = g++
CCFLAGS = -Wall
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CCFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~