#include "INCLUDE/cpu.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Cpu::Cpu()
{
}

// Destructeur
Cpu::~Cpu()
{
}

// ********************************************************
// ACCESSEURS
// ********************************************************

// Accesseur registre A
std::uint8_t Cpu::getRegisterA()
{
    return m_registers.s8bits.a;
}

// Accesseur registre B
std::uint8_t Cpu::getRegisterB()
{
    return m_registers.s8bits.b;
}

// Accesseur registre C
std::uint8_t Cpu::getRegisterC()
{
    return m_registers.s8bits.c;
}

// Accesseur registre D
std::uint8_t Cpu::getRegisterD()
{
    return m_registers.s8bits.d;
}

// Accesseur registre E
std::uint8_t Cpu::getRegisterE()
{
    return m_registers.s8bits.e;
}

// Accesseur registre F
std::uint8_t Cpu::getRegisterF()
{
    return m_registers.s8bits.f;
}

// Accesseur registre H
std::uint8_t Cpu::getRegisterH()
{
    return m_registers.s8bits.h;
}

// Accesseur registre L
std::uint8_t Cpu::getRegisterL()
{
    return m_registers.s8bits.l;
}

// Accesseur registre AF
std::uint16_t Cpu::getRegisterAF()
{
    return m_registers.s16bits.af;
}

// Accesseur registre BC
std::uint16_t Cpu::getRegisterBC()
{
    return m_registers.s16bits.bc;
}

// Accesseur registre DE
std::uint16_t Cpu::getRegisterDE()
{
    return m_registers.s16bits.de;
}

// Accesseur registre HL
std::uint16_t Cpu::getRegisterHL()
{
    return m_registers.s16bits.hl;
}

// Accesseur flag Z
std::uint8_t Cpu::getFlagZ()
{
    return m_registers.sFlags.z;
}

// Accesseur flag N
std::uint8_t Cpu::getFlagN()
{
    return m_registers.sFlags.n;
}

// Accesseur flag H
std::uint8_t Cpu::getFlagH()
{
    return m_registers.sFlags.h;
}

// Accesseur flag C
std::uint8_t Cpu::getFlagC()
{
    return m_registers.sFlags.c;
}

// Accesseur flag RES
std::uint8_t Cpu::getFlagRES()
{
    return m_registers.sFlags.res;
}

// Accesseur registre PC
std::uint16_t Cpu::getRegisterPC()
{
    return m_pc;
}

// Accesseur registre SP
std::uint16_t Cpu::getRegisterSP()
{
    return m_sp;
}
