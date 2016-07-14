#include "CORE/INCLUDE/cpu.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Cpu::Cpu()
{
    // Initialisation des registres
    initRegisters();

    // Initialisation des masques et identifiants des opcodes
    initOpcodesDesc();
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

// Accesseur structure registres
te_registers Cpu::getRegisters()
{
    return m_registers;
}


// ********************************************************
// INITIALISATION DES REGISTRES
// ********************************************************

void Cpu::initRegisters()
{
    // Initialisation des registres
    m_registers.s8bits.a    = 0x01;
    m_registers.s8bits.f    = 0xB0;
    m_registers.s16bits.bc  = 0x0013;
    m_registers.s16bits.de  = 0x00D8;
    m_registers.s16bits.hl  = 0x014D;
    m_sp                    = 0xFFFE;
    //m_pc                    = 0x100;
    m_pc                    = 0x150;
}


// ********************************************************
// INITIALISATION DES MASQUES ET IDENTIFIANTS DES OPCODES
// ********************************************************

void Cpu::initOpcodesDesc()
{
    // Initialisation des masques et identifiants des opcodes 8 bits
    m_opcodesDesc.masque8bits[0]  = 0xFF; m_opcodesDesc.id8bits[0]  = 0x00;         // NOP
    m_opcodesDesc.masque8bits[1]  = 0xFF; m_opcodesDesc.id8bits[1]  = 0x08;         // LN (N),SP
    m_opcodesDesc.masque8bits[2]  = 0xCF; m_opcodesDesc.id8bits[2]  = 0x01;         // LD R,N
    m_opcodesDesc.masque8bits[3]  = 0xCF; m_opcodesDesc.id8bits[3]  = 0x09;         // ADD HL,R
    m_opcodesDesc.masque8bits[4]  = 0xEF; m_opcodesDesc.id8bits[4]  = 0x02;         // LD (R),A
    m_opcodesDesc.masque8bits[5]  = 0xEF; m_opcodesDesc.id8bits[5]  = 0x0A;         // LD A,(R)
    m_opcodesDesc.masque8bits[6]  = 0xCF; m_opcodesDesc.id8bits[6]  = 0x03;         // INC R
    m_opcodesDesc.masque8bits[7]  = 0xCF; m_opcodesDesc.id8bits[7]  = 0x0B;         // DEC R
    m_opcodesDesc.masque8bits[8]  = 0xC7; m_opcodesDesc.id8bits[8]  = 0x04;         // INC D
    m_opcodesDesc.masque8bits[9]  = 0xC7; m_opcodesDesc.id8bits[9]  = 0x05;         // DEC D
    m_opcodesDesc.masque8bits[10] = 0xC7; m_opcodesDesc.id8bits[10] = 0x06;         // LD D,N
    m_opcodesDesc.masque8bits[11] = 0xF7; m_opcodesDesc.id8bits[11] = 0x07;         // RdCA
    m_opcodesDesc.masque8bits[12] = 0xF7; m_opcodesDesc.id8bits[12] = 0x17;         // RdA
    m_opcodesDesc.masque8bits[13] = 0xFF; m_opcodesDesc.id8bits[13] = 0x10;         // STOP
    m_opcodesDesc.masque8bits[14] = 0xFF; m_opcodesDesc.id8bits[14] = 0x18;         // JR N
    m_opcodesDesc.masque8bits[15] = 0xE7; m_opcodesDesc.id8bits[15] = 0x20;         // JR F,N
    m_opcodesDesc.masque8bits[16] = 0xFF; m_opcodesDesc.id8bits[16] = 0x22;         // LDI (HL),A
    m_opcodesDesc.masque8bits[17] = 0xFF; m_opcodesDesc.id8bits[17] = 0x2A;         // LDI A,(HL)
    m_opcodesDesc.masque8bits[18] = 0xFF; m_opcodesDesc.id8bits[18] = 0x32;         // LDD (HL),A
    m_opcodesDesc.masque8bits[19] = 0xFF; m_opcodesDesc.id8bits[19] = 0x3A;         // LDD A,(HL)
    m_opcodesDesc.masque8bits[20] = 0xFF; m_opcodesDesc.id8bits[20] = 0x27;         // DAA
    m_opcodesDesc.masque8bits[21] = 0xFF; m_opcodesDesc.id8bits[21] = 0x2F;         // CPL
    m_opcodesDesc.masque8bits[22] = 0xFF; m_opcodesDesc.id8bits[22] = 0x37;         // SCF
    m_opcodesDesc.masque8bits[23] = 0xFF; m_opcodesDesc.id8bits[23] = 0x3F;         // CCF
    m_opcodesDesc.masque8bits[24] = 0xC0; m_opcodesDesc.id8bits[24] = 0x40;         // LD D,D
    m_opcodesDesc.masque8bits[25] = 0xFF; m_opcodesDesc.id8bits[25] = 0x76;         // HALT
    m_opcodesDesc.masque8bits[26] = 0xC0; m_opcodesDesc.id8bits[26] = 0x80;         // ALU A,D
    m_opcodesDesc.masque8bits[27] = 0xC7; m_opcodesDesc.id8bits[27] = 0xC6;         // ALU A,N
    m_opcodesDesc.masque8bits[28] = 0xCF; m_opcodesDesc.id8bits[28] = 0xC1;         // POP R
    m_opcodesDesc.masque8bits[29] = 0xCF; m_opcodesDesc.id8bits[29] = 0xC5;         // PUSH R
    m_opcodesDesc.masque8bits[30] = 0xC7; m_opcodesDesc.id8bits[30] = 0xC7;         // RST N
    m_opcodesDesc.masque8bits[31] = 0xE7; m_opcodesDesc.id8bits[31] = 0xC0;         // RET F
    m_opcodesDesc.masque8bits[32] = 0xFF; m_opcodesDesc.id8bits[32] = 0xC9;         // RET
    m_opcodesDesc.masque8bits[33] = 0xFF; m_opcodesDesc.id8bits[33] = 0xD9;         // RETI
    m_opcodesDesc.masque8bits[34] = 0xE7; m_opcodesDesc.id8bits[34] = 0xC2;         // JP F,N
    m_opcodesDesc.masque8bits[35] = 0xFF; m_opcodesDesc.id8bits[35] = 0xC3;         // JP N
    m_opcodesDesc.masque8bits[36] = 0xE7; m_opcodesDesc.id8bits[36] = 0xC4;         // CALL F,N
    m_opcodesDesc.masque8bits[37] = 0xFF; m_opcodesDesc.id8bits[37] = 0xCD;         // CALL N
    m_opcodesDesc.masque8bits[38] = 0xFF; m_opcodesDesc.id8bits[38] = 0xE8;         // ADD SP,N
    m_opcodesDesc.masque8bits[39] = 0xFF; m_opcodesDesc.id8bits[39] = 0xF8;         // LD HL,SP+N
    m_opcodesDesc.masque8bits[40] = 0xFF; m_opcodesDesc.id8bits[40] = 0xE0;         // LD (FF00+N),A
    m_opcodesDesc.masque8bits[41] = 0xFF; m_opcodesDesc.id8bits[41] = 0xF0;         // LD A,(FF00+N)
    m_opcodesDesc.masque8bits[42] = 0xFF; m_opcodesDesc.id8bits[42] = 0xE2;         // LD (C),A
    m_opcodesDesc.masque8bits[43] = 0xFF; m_opcodesDesc.id8bits[43] = 0xF2;         // LD A,(C)
    m_opcodesDesc.masque8bits[44] = 0xFF; m_opcodesDesc.id8bits[44] = 0xEA;         // LD (N),A
    m_opcodesDesc.masque8bits[45] = 0xFF; m_opcodesDesc.id8bits[45] = 0xFA;         // LD A,(N)
    m_opcodesDesc.masque8bits[46] = 0xFF; m_opcodesDesc.id8bits[46] = 0xE9;         // JP HL
    m_opcodesDesc.masque8bits[47] = 0xFF; m_opcodesDesc.id8bits[47] = 0xF9;         // LD SP,HL
    m_opcodesDesc.masque8bits[48] = 0xFF; m_opcodesDesc.id8bits[48] = 0xF3;         // DI
    m_opcodesDesc.masque8bits[49] = 0xFF; m_opcodesDesc.id8bits[49] = 0xFB;         // EI
    m_opcodesDesc.masque8bits[50] = 0xFF; m_opcodesDesc.id8bits[50] = 0xCB;         // 16b opcode

    // Initialisation des masques et identifiants des LSW des opcodes 16 bits
    m_opcodesDesc.masque16bitsLSW[0] = 0xF0; m_opcodesDesc.id16bitsLSW[0] = 0x00;   // RdC D
    m_opcodesDesc.masque16bitsLSW[1] = 0xF0; m_opcodesDesc.id16bitsLSW[1] = 0x10;   // Rd D
    m_opcodesDesc.masque16bitsLSW[2] = 0xF0; m_opcodesDesc.id16bitsLSW[2] = 0x20;   // SdA D
    m_opcodesDesc.masque16bitsLSW[3] = 0xF8; m_opcodesDesc.id16bitsLSW[3] = 0x30;   // SWAP D
    m_opcodesDesc.masque16bitsLSW[4] = 0xF8; m_opcodesDesc.id16bitsLSW[4] = 0x38;   // SRL D
    m_opcodesDesc.masque16bitsLSW[5] = 0xC0; m_opcodesDesc.id16bitsLSW[5] = 0x40;   // BIT N,D
    m_opcodesDesc.masque16bitsLSW[6] = 0xC0; m_opcodesDesc.id16bitsLSW[6] = 0x80;   // RES N,D
    m_opcodesDesc.masque16bitsLSW[7] = 0xC0; m_opcodesDesc.id16bitsLSW[7] = 0xC0;   // SET N,D
}


// ********************************************************
// DECODAGE D'UN OPCODE
// ********************************************************

std::uint8_t Cpu::decodeOpcode(std::uint8_t ai_opcode)
{
    std::uint16_t w_id;

    for(std::uint16_t w_i = 0; w_i < CPU_NB_OPCODES_8_BITS; w_i++)
    {
        // Recherche de l'ID correspondant à l'opcode
        w_id = (m_opcodesDesc.masque8bits[w_i] & ai_opcode);

        if(w_id == m_opcodesDesc.id8bits[w_i])
        {
           break;
        }
    }

    return w_id;
}


// ********************************************************
// EXECUTION D'UN OPCODE
// ********************************************************

void Cpu::executeOpcode(std::uint8_t* ai_opcode)
{
    // Variable locale
    std::uint8_t w_data8bits     = 0;
    std::uint8_t w_register8bits = 0;

    // On récupère l'ID de l'opcode à executer
    std::uint8_t w_id = decodeOpcode(*ai_opcode);

    // On exécute l'opcode correspondant
    switch(w_id)
    {
        case 0x00:  // NOP
        {
            // Mise à jour de PC
            m_pc = m_pc + 1;

            // On passe 4 cycles
            std::cout << "NOP" << std::endl;

            break;
        }

        case 0x06:  // LD D,N
        {
            // Récupération du registre
            w_register8bits = ( (*ai_opcode) & 0x38 ) >> 3;

            // Récupération de la valeur à charger dans le registre
            w_data8bits = *(ai_opcode + 1);

            // Chargement de la valeur dans le registre
            if (w_register8bits == 0) m_registers.s8bits.b = w_data8bits;
            else if (w_register8bits == 1) m_registers.s8bits.c = w_data8bits;
            else if (w_register8bits == 2) m_registers.s8bits.d = w_data8bits;
            else if (w_register8bits == 3) m_registers.s8bits.e = w_data8bits;
            else if (w_register8bits == 4) m_registers.s8bits.h = w_data8bits;
            else if (w_register8bits == 5) m_registers.s8bits.l = w_data8bits;
            else if (w_register8bits == 7) m_registers.s8bits.a = w_data8bits;

            // Mise à jour de PC
            m_pc = m_pc + 2;

            // On passe 4 (ou 8 si HL) cycles
            std::cout << "LD ";
            if (w_register8bits == 0) std::cout << "B";
            else if (w_register8bits == 1) std::cout << "C";
            else if (w_register8bits == 2) std::cout << "D";
            else if (w_register8bits == 3) std::cout << "E";
            else if (w_register8bits == 4) std::cout << "H";
            else if (w_register8bits == 5) std::cout << "L";
            else if (w_register8bits == 7) std::cout << "A";
            std::cout << std::hex << std::uppercase << ",0x" << static_cast<std::uint16_t>(w_data8bits) << std::endl;

            break;
        }

        default:
        {
            // Mise à jour de PC
            m_pc = m_pc + 1;

            std::cout << std::hex << std::uppercase << "Opcode 0x" << static_cast<std::uint16_t>(*ai_opcode) << " non reconnu." << std::endl;

            break;
        }
    }
}
