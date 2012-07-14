EXEC = term-core
CXX = g++
CCFLAGS = -Wall -O3
LDFLAGS = -ldl -lrt -lpthread
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)
	cd lib ; make launcher.td ; make dir.td

$(EXEC): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(CCFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f $(EXEC) $(OBJECTS)
	-rm *~
	cd lib ; make clean

install:
	-mkdir -p $(DESTDIR)/bin
	cp $(EXEC) $(DESTDIR)/bin
	-mkdir -p $(DESTDIR)/lib/term-do/plugins
	cp lib/*.td $(DESTDIR)/lib/term-do/plugins
