CXX=g++
CFLAGS=-I/usr/include -Wall -I. -std=c++11
LDFLAGS=-L/usr/lib/i386-linux-gnu/ -lm

.PHONY: all clean

PROG=gbterm
OBJS=   OBJ/term.o              \
        OBJ/cpu.o               \
        OBJ/gameboy.o           \


$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
    
OBJ/term.o: GUI/Term/SRC_CPP/term.cpp GUI/Term/INCLUDE/term.h
	$(CXX) $(CFLAGS) -c $< -o $@

OBJ/cpu.o: CORE/SRC_CPP/cpu.cpp CORE/INCLUDE/cpu.h
	$(CXX) $(CFLAGS) -c $< -o $@
    
OBJ/gameboy.o: CORE/SRC_CPP/gameboy.cpp CORE/INCLUDE/gameboy.h
	$(CXX) $(CFLAGS) -c $< -o $@
    
all: $(PROG)

clean:
	rm -rf ./OBJ/*.o
	rm -f $(PROG)
   