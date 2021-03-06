#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <cstdint>

#include "shared_data.h"
#include "mpu.h"

#define CPU_NB_OPCODES_8_BITS   50u
#define CPU_NB_OPCODES_16_BITS  8u

#define CPU_DAA_TABLE_NB        13u

// Adresse des interruptions
#define CPU_VBLANK_ADDRESS      0x0040
#define CPU_LCD_STAT_ADDRESS    0x0048
#define CPU_TIMER_ADDRESS       0x0050
#define CPU_SERIAL_ADDRESS      0x0058
#define CPU_JOYPAD_ADDRESS      0x0060

// Union codant les différents registres de la gameboy
union tu_registers
{
    // Modélisation du registre F (flags)
    struct ts_Flags
    {
        std::uint8_t 	res:4u,
                        c:1u,
                        h:1u,
                        n:1u,
                        z:1u;
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


class Cpu
{
public:
    // Enumération des différentes interruptions
    enum te_interupts
    {
        E_VBLANK    = 0u,
        E_LCD_STAT,
        E_TIMER,
        E_SERIAL,
        E_JOYPAD
    };

public:
    explicit Cpu(Mpu* const aip_mpu);
    ~Cpu();

    // Initialisation des registres
    void initRegisters();

    // Exécution d'un opcode
    void executeOpcode(const std::uint16_t ai_opcodeIdx);

    // Exécution d'une interruption
    void executeInterrupt(const te_interupts ai_interrupt);

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

    // Accesseur sur le nombre de cycles de l'instruction courante
    std::uint8_t getNbCyccles() const;

    // Accesseur sur le flag d'activation des interruptions
    std::uint8_t getRegisterIME() const;

    // Accesseur sur l'état de pause du CPU
    bool getIsHalted() const;

private:
    // Structure des masques et identifiants des opcodes
    struct ts_opcodesDesc
    {
        std::uint8_t 	masque8bits[CPU_NB_OPCODES_8_BITS];
        std::uint8_t 	id8bits[CPU_NB_OPCODES_8_BITS];
        void            (Cpu::*execute8bits[CPU_NB_OPCODES_8_BITS])();
        std::uint8_t 	masque16bitsLSW[CPU_NB_OPCODES_16_BITS];
        std::uint8_t 	id16bitsLSW[CPU_NB_OPCODES_16_BITS];
        void            (Cpu::*execute16bits[CPU_NB_OPCODES_16_BITS])();
    };

    // Structure des données nécessaires à l'exécution de l'instruction DAA
    struct ts_daaTableDesc
    {
        std::uint8_t    flagN;
        std::uint8_t    flagH;
        std::uint8_t    flagCCur;
        std::uint8_t    flagCNew;
        std::uint8_t    minAMsw;
        std::uint8_t    maxAMsw;
        std::uint8_t    minALsw;
        std::uint8_t    maxALsw;
        std::uint8_t    addValue;
    };

private:
    // Initialisation des masques et identifiants des opcodes
    void _initOpcodesDesc();

    // Décodage d'un mnémonique
    bool _decodeMnemonic(const std::uint8_t ai_mnemo);

    // Décodage d'un registre 8 bits
    void _decodeRegister8Bits(const std::uint8_t ai_registerMask, std::uint8_t* &aop_register8bits);

    // Décodage d'un registre 16 bits
    void _decodeRegister16Bits(const std::uint8_t ai_registerMask, std::uint16_t* &aop_register16bits);

    // Décodage et exécution d'une opération ALU
    void _decodeAndRunALU(const std::uint8_t ai_aluMask, const std::uint8_t ai_operand);


    // LISTE DES INSTRUCTIONS 8 BITS
    // *****************************
    void _nop();
    void _ld_n_sp();
    void _ld_r_n();
    void _add_hl_r();
    void _ld_r_a();
    void _ld_a_r();
    void _inc_r();
    void _dec_r();
    void _inc_d();
    void _dec_d();
    void _ld_d_n();
    void _rdca();
    void _rda();
    void _stop();
    void _jr_n();
    void _jr_f_n();
    void _ldi_hl_a();
    void _ldi_a_hl();
    void _ldd_hl_a();
    void _ldd_a_hl();
    void _daa();
    void _cpl();
    void _scf();
    void _ccf();
    void _ld_d_d();
    void _halt();
    void _alu_a_d();
    void _alu_a_n();
    void _pop_r();
    void _push_r();
    void _rst_n();
    void _ret_f();
    void _ret();
    void _reti();
    void _jp_f_n();
    void _jp_n();
    void _call_f_n();
    void _call_n();
    void _add_sp_n();
    void _ld_hl_sp_plus_n();
    void _ld_ff00_plus_n_a();
    void _ld_a_ff00_plus_n();
    void _ld_c_a();
    void _ld_a_c();
    void _ld_n_a();
    void _ld_a_n();
    void _jp_hl();
    void _ld_sp_hl();
    void _di();
    void _ei();
    void _execute16bOpcode();

    // LISTE DES INSTRUCTIONS 16 BITS
    // ******************************
    void _rdc_d();
    void _rd_d();
    void _sda_d();
    void _swap_d();
    void _srl_d();
    void _bit_n_d();
    void _res_n_d();
    void _set_n_d();


private:
    std::uint16_t   m_opcodeIdx;                        // Opcode courant à exécuter
    std::uint8_t    m_nbCycles;                         // Nombre de cycles de l'opcode courant

    ts_opcodesDesc  m_opcodesDesc;                      // Masque et identifiant des opcodes

    tu_registers 	m_registers;                        // Registres 8-16 bits
    std::uint16_t	m_pc;                               // Program Counter
    std::uint16_t	m_sp;                               // Stack Pointer
    std::uint8_t    m_ime;                              // Interrupt Master Enable flag

    Mpu*            mp_mpu;                             // Pointeur vers la mémoire

    bool            m_isHalted;                         // VRAI si l'instruction HALT a été exécutée

    ts_daaTableDesc m_daaTable[CPU_DAA_TABLE_NB];       // Table des données nécessaires à l'exécution de l'instruction DAA
};

#endif // CPU_H
