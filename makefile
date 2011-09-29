CXX = g++
CCFLAGS = -Wall -O3
LDFLAGS = -ldl -lrt
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	cd lib ; make bootstrap ;  make launcher.so ; make dir.so
	# -ln -fs *.o daemon/
	cd daemon ; make
	cd standalone ; make

$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(CCFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~
	cd lib ; make clean
	cd daemon ; make clean
	cd standalone ; make clean
