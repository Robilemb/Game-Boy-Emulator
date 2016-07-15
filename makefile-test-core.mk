CXX=g++
CFLAGS=-I/usr/include -Wall -I. -std=c++11
LDFLAGS=-L/usr/lib/i386-linux-gnu/ -lm

.PHONY: all clean

PROG=test_core
OBJS=   OBJ/core_tu.o           \
        OBJ/cpu.o               \

$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
    
OBJ/core_tu.o: CORE/TU/core_tu.cpp CORE/INCLUDE/cpu.h
	$(CXX) $(CFLAGS) -c $< -o $@

OBJ/cpu.o: CORE/SRC_CPP/cpu.cpp CORE/INCLUDE/cpu.h
	$(CXX) $(CFLAGS) -c $< -o $@
    
all: $(PROG)

clean:
	rm -rf ./OBJ/*.o
	rm -f $(PROG)
   