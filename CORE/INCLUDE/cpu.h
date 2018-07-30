#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <cstdint>

#include "mpu.h"

#define CPU_NB_OPCODES_8_BITS   51
#define CPU_NB_OPCODES_16_BITS  8

// Union codant les différents registres de la gameboy
union tu_registers
{
    // Modélisation du registre F (flags)
    struct ts_Flags
    {
        std::uint8_t 	res:4,
                        c:1,
                        h:1,
                        n:1,
                        z:1;
    } sFlags;

    // Registres 8 bits
    struct ts_8bits
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
    struct ts_16bits
    {
        std::uint16_t af;
        std::uint16_t bc;
        std::uint16_t de;
        std::uint16_t hl;
    } s16bits;
};


// Structure des masques et identifiants des opcodes
struct ts_opcodesDesc
{
    std::uint8_t 	masque8bits[CPU_NB_OPCODES_8_BITS];
    std::uint8_t 	id8bits[CPU_NB_OPCODES_8_BITS];
    std::uint8_t 	masque16bitsLSW[CPU_NB_OPCODES_16_BITS];
    std::uint8_t 	id16bitsLSW[CPU_NB_OPCODES_16_BITS];
};


class Cpu
{
public:
    explicit Cpu(Mpu* ai_mpu);
    ~Cpu();

    // Initialisation des registres
    void initRegisters();

    // Exécution d'un opcode
    void executeOpcode(const std::uint16_t ai_opcodeIdx);

    // Accesseur registre A
    std::uint8_t getRegisterA() const;

    // Accesseur registre B
    std::uint8_t getRegisterB() const;

    // Accesseur registre C
    std::uint8_t getRegisterC() const;

    // Accesseur registre D
    std::uint8_t getRegisterD() const;

    // Accesseur registre E
    std::uint8_t getRegisterE() const;

    // Accesseur registre F
    std::uint8_t getRegisterF() const;

    // Accesseur registre H
    std::uint8_t getRegisterH() const;

    // Accesseur registre L
    std::uint8_t getRegisterL() const;

    // Accesseur registre AF
    std::uint16_t getRegisterAF() const;

    // Accesseur registre BC
    std::uint16_t getRegisterBC() const;

    // Accesseur registre DE
    std::uint16_t getRegisterDE() const;

    // Accesseur registre HL
    std::uint16_t getRegisterHL() const;

    // Accesseur flag Z
    std::uint8_t getFlagZ() const;

    // Accesseur flag N
    std::uint8_t getFlagN() const;

    // Accesseur flag H
    std::uint8_t getFlagH() const;

    // Accesseur flag C
    std::uint8_t getFlagC() const;

    // Accesseur flag RES
    std::uint8_t getFlagRES() const;

    // Accesseur registre PC
    std::uint16_t getRegisterPC() const;

    // Accesseur registre SP
    std::uint16_t getRegisterSP() const;

    // Accesseur structure registres
    tu_registers getRegisters() const;

private:
    // Initialisation des masques et identifiants des opcodes
    void initOpcodesDesc();

    // Décodage d'un opcode
    std::uint8_t _decodeOpcode(const std::uint8_t ai_opcode) const;


    // LISTE DES INSTRUCTIONS
    // **********************
    void _decodeNop();
    void _decodeLoad8bits(const std::uint8_t ai_id, const std::uint16_t ai_opcodeIdx);
    void _decodeLoad16bits(const std::uint8_t ai_id, const std::uint16_t ai_opcodeIdx);
    void _decodeJump(const std::uint8_t ai_id, const std::uint16_t ai_opcodeIdx);
    void _decodeInc(const std::uint8_t ai_id, const std::uint16_t ai_opcodeIdx);
    void _decodeDec(const std::uint8_t ai_id, const std::uint16_t ai_opcodeIdx);

    // Récupération d'un registre 8/16 bits en fonction de son masque
    // 0 1 2 3 4 5    6 7
    // B C D E H L (HL) A
    void _decodeRegister8Bits(const std::uint8_t ai_registerMask, std::string &ai_sReg, std::uint8_t* &aop_register8bits, std::uint16_t* &aop_register16bits);

    // Récupération d'un registre 16 bits en fonction de son masque et de l'opcode courant
    //  0  1  2     3
    // BC DE HL SP/AF
    std::uint16_t* _decodeRegister16Bits(const std::uint8_t ai_registerMask, const std::uint8_t ai_opcodeIdx, std::string &ai_sReg);


private:
    ts_opcodesDesc  m_opcodesDesc;              // Masque et identifiant des opcodes

    tu_registers 	m_registers;				// Registres 8-16 bits
    std::uint16_t	m_pc;						// Program Counter
    std::uint16_t	m_sp;						// Stack Pointer

    Mpu*            mp_mpu;                     // Pointeur vers la mémoire
};


#endif // CPU_H
