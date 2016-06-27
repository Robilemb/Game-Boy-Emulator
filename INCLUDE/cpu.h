#ifndef CPU_H
#define CPU_H

#include <iostream>

// Taille de la mémoire = 64 Ko
/*! \brief	Taille de la mémoire
 *
 * La gameboy a 64 ko de mémoire.
 * Cf le mapping ci-dessous.
 */
#define CPU_MEMORY_SIZE 			65535

/*! \brief	Union codant les différents registres de la gameboy
 *
 */
typedef union
{
    // Modélisation du registre F (flags)
    struct s_Flags
    {
        std::uint8_t 	res:4,
                        c:1,
                        h:1,
                        n:1,
                        z:1;
    } sFlags;

    // Registres 8 bits
    struct s_8bits
    {
        std::uint8_t f;
        std::uint8_t a;
        std::uint8_t c;
        std::uint8_t b;
        std::uint8_t e;
        std::uint8_t d;
        std::uint8_t l;
        std::uint8_t h;
    } s8bits;

    // Registres 16 bits (concaténation des registres 8 bits deux à deux)
    struct s_16bits
    {
        std::uint16_t af;
        std::uint16_t bc;
        std::uint16_t de;
        std::uint16_t hl;
    } s16bits;
} te_registers;


class Cpu
{

public:
    explicit Cpu();
    ~Cpu();

    // Accesseur registre A
    std::uint8_t getRegisterA();

    // Accesseur registre B
    std::uint8_t getRegisterB();

    // Accesseur registre C
    std::uint8_t getRegisterC();

    // Accesseur registre D
    std::uint8_t getRegisterD();

    // Accesseur registre E
    std::uint8_t getRegisterE();

    // Accesseur registre F
    std::uint8_t getRegisterF();

    // Accesseur registre H
    std::uint8_t getRegisterH();

    // Accesseur registre L
    std::uint8_t getRegisterL();

    // Accesseur registre AF
    std::uint16_t getRegisterAF();

    // Accesseur registre BC
    std::uint16_t getRegisterBC();

    // Accesseur registre DE
    std::uint16_t getRegisterDE();

    // Accesseur registre HL
    std::uint16_t getRegisterHL();

    // Accesseur flag Z
    std::uint8_t getFlagZ();

    // Accesseur flag N
    std::uint8_t getFlagN();

    // Accesseur flag H
    std::uint8_t getFlagH();

    // Accesseur flag C
    std::uint8_t getFlagC();

    // Accesseur flag RES
    std::uint8_t getFlagRES();

    // Accesseur registre PC
    std::uint16_t getRegisterPC();

    // Accesseur registre SP
    std::uint16_t getRegisterSP();

private:
    te_registers 	m_registers;				// Registres 8-16 bits
    std::uint16_t	m_pc;						// Program Counter
    std::uint16_t	m_sp;						// Stack Pointer
    std::uint8_t 	m_memory[CPU_MEMORY_SIZE];	// Mémoire

};


#endif // CPU_H
