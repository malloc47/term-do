EXEC = term-do 
CXX = g++
CCFLAGS = -Wall
LDFLAGS = -ldl
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)
	cd lib ; make launcher.so ; make dir.so

$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(CCFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~
	cd lib ; make clean
