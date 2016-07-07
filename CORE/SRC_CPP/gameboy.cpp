#include "CORE/INCLUDE/gameboy.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy()
{
    mp_cpu = new Cpu();
}

// Destructeur
Gameboy::~Gameboy()
{
    delete mp_cpu;
}


// ********************************************************
// ACCESSEURS
// ********************************************************

Cpu* Gameboy::getCpu()
{
    return mp_cpu;
}
