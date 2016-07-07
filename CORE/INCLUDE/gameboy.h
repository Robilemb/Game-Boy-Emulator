#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"

class Gameboy
{
public:
    explicit Gameboy();
    ~Gameboy();

    Cpu* getCpu();

private:
    Cpu* mp_cpu;
};

#endif // GAMEBOY_H
