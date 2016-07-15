#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <cstdint>

#define CPU_NB_OPCODES_8_BITS   51
#define CPU_NB_OPCODES_16_BITS  8

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


// Structure des masques et identifiants des opcodes
typedef struct
{
    std::uint8_t masque8bits[CPU_NB_OPCODES_8_BITS];
    std::uint8_t id8bits[CPU_NB_OPCODES_8_BITS];
    std::uint8_t masque16bitsLSW[CPU_NB_OPCODES_16_BITS];
    std::uint8_t id16bitsLSW[CPU_NB_OPCODES_16_BITS];
} sOpcodesDesc;


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

    // Accesseur structure registres
    te_registers getRegisters();

    // Initialisation des registres
    void initRegisters();

    // Exécution d'un opcode
    void 				executeOpcode(std::uint8_t* ai_opcode);

    std::string			showInstruction(std::uint8_t* ai_mem);

private:
    // Décodage d'un opcode
    std::uint8_t 		decodeOpcode(std::uint8_t ai_opcode);

    std::string			decodeInstr(std::uint8_t* ai_mem, bool ai_exec);

    // LISTE DES INSTRUCTIONS GEREES
    // *****************************
    std::string			__decodeNop(std::uint8_t* ai_mem, bool ai_exec);
    std::string			__decodeLoad(std::uint8_t* ai_mem, bool ai_exec);

    // Initialisation des masques et identifiants des opcodes
    void initOpcodesDesc();

private:
    te_registers 	m_registers;				// Registres 8-16 bits
    std::uint16_t	m_pc;						// Program Counter
    std::uint16_t	m_sp;						// Stack Pointer

    sOpcodesDesc    m_opcodesDesc;              // Masque et identifiant des opcodes

};


#endif // CPU_H
