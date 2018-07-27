#include <string>
#include "CORE/INCLUDE/cpu.h"


// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Cpu::Cpu(Mpu* ai_mpu)
{
    // Récupération du pointeur vers la mémoire
    mp_mpu = ai_mpu;

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
std::uint8_t Cpu::getRegisterA() const
{
    return m_registers.s8bits.a;
}

// Accesseur registre B
std::uint8_t Cpu::getRegisterB() const
{
    return m_registers.s8bits.b;
}

// Accesseur registre C
std::uint8_t Cpu::getRegisterC() const
{
    return m_registers.s8bits.c;
}

// Accesseur registre D
std::uint8_t Cpu::getRegisterD() const
{
    return m_registers.s8bits.d;
}

// Accesseur registre E
std::uint8_t Cpu::getRegisterE() const
{
    return m_registers.s8bits.e;
}

// Accesseur registre F
std::uint8_t Cpu::getRegisterF() const
{
    return m_registers.s8bits.f;
}

// Accesseur registre H
std::uint8_t Cpu::getRegisterH() const
{
    return m_registers.s8bits.h;
}

// Accesseur registre L
std::uint8_t Cpu::getRegisterL() const
{
    return m_registers.s8bits.l;
}

// Accesseur registre AF
std::uint16_t Cpu::getRegisterAF() const
{
    return m_registers.s16bits.af;
}

// Accesseur registre BC
std::uint16_t Cpu::getRegisterBC() const
{
    return m_registers.s16bits.bc;
}

// Accesseur registre DE
std::uint16_t Cpu::getRegisterDE() const
{
    return m_registers.s16bits.de;
}

// Accesseur registre HL
std::uint16_t Cpu::getRegisterHL() const
{
    return m_registers.s16bits.hl;
}

// Accesseur flag Z
std::uint8_t Cpu::getFlagZ() const
{
    return m_registers.sFlags.z;
}

// Accesseur flag N
std::uint8_t Cpu::getFlagN() const
{
    return m_registers.sFlags.n;
}

// Accesseur flag H
std::uint8_t Cpu::getFlagH() const
{
    return m_registers.sFlags.h;
}

// Accesseur flag C
std::uint8_t Cpu::getFlagC() const
{
    return m_registers.sFlags.c;
}

// Accesseur flag RES
std::uint8_t Cpu::getFlagRES() const
{
    return m_registers.sFlags.res;
}

// Accesseur registre PC
std::uint16_t Cpu::getRegisterPC() const
{
    return m_pc;
}

// Accesseur registre SP
std::uint16_t Cpu::getRegisterSP() const
{
    return m_sp;
}

// Accesseur structure registres
tu_registers Cpu::getRegisters() const
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
    m_pc                    = 0x100;
}


// ********************************************************
// INITIALISATION DES MASQUES ET IDENTIFIANTS DES OPCODES
// ********************************************************

void Cpu::initOpcodesDesc()
{
    // Initialisation des masques et identifiants des opcodes 8 bits
    m_opcodesDesc.masque8bits[0]  = 0xFF; m_opcodesDesc.id8bits[0]  = 0x00; m_opcodesDesc.instr8bits[0] = "NOP";	// NOP
    m_opcodesDesc.masque8bits[1]  = 0xFF; m_opcodesDesc.id8bits[1]  = 0x08; m_opcodesDesc.instr8bits[1] = "LN";    	// LN (N),SP
    m_opcodesDesc.masque8bits[2]  = 0xCF; m_opcodesDesc.id8bits[2]  = 0x01; m_opcodesDesc.instr8bits[2] = "LD";     // LD R,N
    m_opcodesDesc.masque8bits[3]  = 0xCF; m_opcodesDesc.id8bits[3]  = 0x09; m_opcodesDesc.instr8bits[3] = "ADD";    // ADD HL,R
    m_opcodesDesc.masque8bits[4]  = 0xEF; m_opcodesDesc.id8bits[4]  = 0x02; m_opcodesDesc.instr8bits[4] = "LD";     // LD (R),A
    m_opcodesDesc.masque8bits[5]  = 0xEF; m_opcodesDesc.id8bits[5]  = 0x0A; m_opcodesDesc.instr8bits[5] = "LD";     // LD A,(R)
    m_opcodesDesc.masque8bits[6]  = 0xCF; m_opcodesDesc.id8bits[6]  = 0x03; m_opcodesDesc.instr8bits[6] = "INC";    // INC R
    m_opcodesDesc.masque8bits[7]  = 0xCF; m_opcodesDesc.id8bits[7]  = 0x0B; m_opcodesDesc.instr8bits[7] = "DEC";    // DEC R
    m_opcodesDesc.masque8bits[8]  = 0xC7; m_opcodesDesc.id8bits[8]  = 0x04; m_opcodesDesc.instr8bits[8] = "INC";    // INC D
    m_opcodesDesc.masque8bits[9]  = 0xC7; m_opcodesDesc.id8bits[9]  = 0x05; m_opcodesDesc.instr8bits[9] = "DEC";    // DEC D
    m_opcodesDesc.masque8bits[10] = 0xC7; m_opcodesDesc.id8bits[10] = 0x06; m_opcodesDesc.instr8bits[10] = "LD";     // LD D,N
    m_opcodesDesc.masque8bits[11] = 0xF7; m_opcodesDesc.id8bits[11] = 0x07; m_opcodesDesc.instr8bits[11] = "RdCA";   // RdCA
    m_opcodesDesc.masque8bits[12] = 0xF7; m_opcodesDesc.id8bits[12] = 0x17; m_opcodesDesc.instr8bits[12] = "RdA";    // RdA
    m_opcodesDesc.masque8bits[13] = 0xFF; m_opcodesDesc.id8bits[13] = 0x10; m_opcodesDesc.instr8bits[13] = "STOP";   // STOP
    m_opcodesDesc.masque8bits[14] = 0xFF; m_opcodesDesc.id8bits[14] = 0x18; m_opcodesDesc.instr8bits[14] = "JR";     // JR N
    m_opcodesDesc.masque8bits[15] = 0xE7; m_opcodesDesc.id8bits[15] = 0x20; m_opcodesDesc.instr8bits[15] = "JR";     // JR F,N
    m_opcodesDesc.masque8bits[16] = 0xFF; m_opcodesDesc.id8bits[16] = 0x22; m_opcodesDesc.instr8bits[16] = "LDI";    // LDI (HL),A
    m_opcodesDesc.masque8bits[17] = 0xFF; m_opcodesDesc.id8bits[17] = 0x2A; m_opcodesDesc.instr8bits[17] = "LDI";    // LDI A,(HL)
    m_opcodesDesc.masque8bits[18] = 0xFF; m_opcodesDesc.id8bits[18] = 0x32; m_opcodesDesc.instr8bits[18] = "LDD";    // LDD (HL),A
    m_opcodesDesc.masque8bits[19] = 0xFF; m_opcodesDesc.id8bits[19] = 0x3A; m_opcodesDesc.instr8bits[19] = "LDD";    // LDD A,(HL)
    m_opcodesDesc.masque8bits[20] = 0xFF; m_opcodesDesc.id8bits[20] = 0x27; m_opcodesDesc.instr8bits[20] = "DAA";    // DAA
    m_opcodesDesc.masque8bits[21] = 0xFF; m_opcodesDesc.id8bits[21] = 0x2F; m_opcodesDesc.instr8bits[21] = "CPL";    // CPL
    m_opcodesDesc.masque8bits[22] = 0xFF; m_opcodesDesc.id8bits[22] = 0x37; m_opcodesDesc.instr8bits[22] = "SCF";    // SCF
    m_opcodesDesc.masque8bits[23] = 0xFF; m_opcodesDesc.id8bits[23] = 0x3F; m_opcodesDesc.instr8bits[23] = "CCF";    // CCF
    m_opcodesDesc.masque8bits[24] = 0xC0; m_opcodesDesc.id8bits[24] = 0x40; m_opcodesDesc.instr8bits[24] = "LD";     // LD D,D
    m_opcodesDesc.masque8bits[25] = 0xFF; m_opcodesDesc.id8bits[25] = 0x76; m_opcodesDesc.instr8bits[25] = "HALT";   // HALT
    m_opcodesDesc.masque8bits[26] = 0xC0; m_opcodesDesc.id8bits[26] = 0x80; m_opcodesDesc.instr8bits[26] = "ALU";    // ALU A,D
    m_opcodesDesc.masque8bits[27] = 0xC7; m_opcodesDesc.id8bits[27] = 0xC6; m_opcodesDesc.instr8bits[27] = "ALU";    // ALU A,N
    m_opcodesDesc.masque8bits[28] = 0xCF; m_opcodesDesc.id8bits[28] = 0xC1; m_opcodesDesc.instr8bits[28] = "POP";    // POP R
    m_opcodesDesc.masque8bits[29] = 0xCF; m_opcodesDesc.id8bits[29] = 0xC5; m_opcodesDesc.instr8bits[29] = "PUSH";   // PUSH R
    m_opcodesDesc.masque8bits[30] = 0xC7; m_opcodesDesc.id8bits[30] = 0xC7; m_opcodesDesc.instr8bits[30] = "RST";    // RST N
    m_opcodesDesc.masque8bits[31] = 0xE7; m_opcodesDesc.id8bits[31] = 0xC0; m_opcodesDesc.instr8bits[31] = "RET";    // RET F
    m_opcodesDesc.masque8bits[32] = 0xFF; m_opcodesDesc.id8bits[32] = 0xC9; m_opcodesDesc.instr8bits[32] = "RET";    // RET
    m_opcodesDesc.masque8bits[33] = 0xFF; m_opcodesDesc.id8bits[33] = 0xD9; m_opcodesDesc.instr8bits[33] = "RETI";   // RETI
    m_opcodesDesc.masque8bits[34] = 0xE7; m_opcodesDesc.id8bits[34] = 0xC2; m_opcodesDesc.instr8bits[34] = "JP";     // JP F,N
    m_opcodesDesc.masque8bits[35] = 0xFF; m_opcodesDesc.id8bits[35] = 0xC3; m_opcodesDesc.instr8bits[35] = "JP";     // JP N
    m_opcodesDesc.masque8bits[36] = 0xE7; m_opcodesDesc.id8bits[36] = 0xC4; m_opcodesDesc.instr8bits[36] = "CALL";   // CALL F,N
    m_opcodesDesc.masque8bits[37] = 0xFF; m_opcodesDesc.id8bits[37] = 0xCD; m_opcodesDesc.instr8bits[37] = "CALL";   // CALL N
    m_opcodesDesc.masque8bits[38] = 0xFF; m_opcodesDesc.id8bits[38] = 0xE8; m_opcodesDesc.instr8bits[38] = "ADD";    // ADD SP,N
    m_opcodesDesc.masque8bits[39] = 0xFF; m_opcodesDesc.id8bits[39] = 0xF8; m_opcodesDesc.instr8bits[39] = "LD";     // LD HL,SP+N
    m_opcodesDesc.masque8bits[40] = 0xFF; m_opcodesDesc.id8bits[40] = 0xE0; m_opcodesDesc.instr8bits[40] = "LD";     // LD (FF00+N),A
    m_opcodesDesc.masque8bits[41] = 0xFF; m_opcodesDesc.id8bits[41] = 0xF0; m_opcodesDesc.instr8bits[41] = "LD";     // LD A,(FF00+N)
    m_opcodesDesc.masque8bits[42] = 0xFF; m_opcodesDesc.id8bits[42] = 0xE2; m_opcodesDesc.instr8bits[42] = "LD";     // LD (C),A
    m_opcodesDesc.masque8bits[43] = 0xFF; m_opcodesDesc.id8bits[43] = 0xF2; m_opcodesDesc.instr8bits[43] = "LD";     // LD A,(C)
    m_opcodesDesc.masque8bits[44] = 0xFF; m_opcodesDesc.id8bits[44] = 0xEA; m_opcodesDesc.instr8bits[44] = "LD";     // LD (N),A
    m_opcodesDesc.masque8bits[45] = 0xFF; m_opcodesDesc.id8bits[45] = 0xFA; m_opcodesDesc.instr8bits[45] = "LD";     // LD A,(N)
    m_opcodesDesc.masque8bits[46] = 0xFF; m_opcodesDesc.id8bits[46] = 0xE9; m_opcodesDesc.instr8bits[46] = "JP";     // JP HL
    m_opcodesDesc.masque8bits[47] = 0xFF; m_opcodesDesc.id8bits[47] = 0xF9; m_opcodesDesc.instr8bits[47] = "LD";     // LD SP,HL
    m_opcodesDesc.masque8bits[48] = 0xFF; m_opcodesDesc.id8bits[48] = 0xF3; m_opcodesDesc.instr8bits[48] = "DI";     // DI
    m_opcodesDesc.masque8bits[49] = 0xFF; m_opcodesDesc.id8bits[49] = 0xFB; m_opcodesDesc.instr8bits[49] = "EI";     // EI
    m_opcodesDesc.masque8bits[50] = 0xFF; m_opcodesDesc.id8bits[50] = 0xCB; m_opcodesDesc.instr8bits[50] = "16BITS"; // 16b opcode

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

std::uint8_t Cpu::decodeOpcode(std::uint8_t ai_opcode) const
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


std::string			Cpu::showInstruction(std::uint16_t ai_idx) const
{
    return decodeInstr(ai_idx);
}

std::uint8_t		Cpu::showInstructionId(std::uint16_t ai_idx) const
{
    std::uint8_t 	w_id = decodeOpcode(mp_mpu->getMemVal(ai_idx));

    return w_id;
}

std::string			Cpu::showInstructionIdStr(std::uint16_t ai_idx) const
{
    std::string		w_str = "";

    std::uint8_t 	w_id = decodeOpcode(mp_mpu->getMemVal(ai_idx));

    for(std::uint16_t w_i = 0; w_i < CPU_NB_OPCODES_8_BITS; w_i++)
    {
        if (w_id == m_opcodesDesc.id8bits[w_i])
        {
            w_str = m_opcodesDesc.instr8bits[w_i];
            break;
        }
    }

    return w_str;
}

// ********************************************************
// EXECUTION D'UN OPCODE
// ********************************************************

void Cpu::executeOpcode(std::uint16_t ai_opcodeIdx)
{
    decodeInstr(ai_opcodeIdx, true);
}

std::string         Cpu::decodeInstr(std::uint16_t ai_idx, bool ai_exec) const {
    if (ai_exec == true) {
        throw std::logic_error("const version of decodeInstr must have ai_exec argument to false");
    }

    // Appel à la fonction non const decodeInstr qui avec le second paramètre false
    // ne modifie aucune donnée interne à la classe, ce qui permet de rester en dehors
    // des cas à comportement indéterminé.
    return const_cast<Cpu*>(this)->decodeInstr(ai_idx, false);
}

std::string			Cpu::decodeInstr(std::uint16_t ai_idx, bool ai_exec)
{
    std::string		w_str = "";

    // On récupère l'ID de l'opcode à executer
    std::uint8_t 	w_id = decodeOpcode(mp_mpu->getMemVal(ai_idx));

    // On exécute l'opcode correspondant
    switch(w_id)
    {
        case 0x00:  // NOP
            w_str = __decodeNop(ai_exec);
            break;

        case 0x06:  // LD D,N
        case 0x02:  // LD (R),A
        case 0x0A:  // LD A,(R)
        case 0x22:  // LDI (HL),A
        case 0x2A:  // LDI A,(HL)
        case 0x32:  // LDD (HL),A
        case 0x3A:  // LDD A,(HL)
        case 0x40:  // LD D,D
        case 0xE0:  // LD (FF00+N),A
        case 0xF0:  // LD A,(FF00+N)
        case 0xE2:  // LD (C),A
        case 0xF2:  // LD A,(C)
        case 0xEA:  // LD (N),A
        case 0xFA:  // LD A,(N)
            w_str = __decodeLoad8bits(w_id, ai_idx, ai_exec);
            break;

        case 0x08:  // LD (N),SP
        case 0x01:  // LD R,N
        case 0xF8:  // LD HL,SP+N
        case 0xF9:  // LD SP,HL
            w_str = __decodeLoad16bits(w_id, ai_idx, ai_exec);
            break;

        case 0xC2:  // JP F,N
        case 0xC3:  // JP N
        case 0xE9:  // JP HL
            w_str = __decodeJump(w_id, ai_idx, ai_exec);
            break;

        case 0x03:  // INC R
        case 0x04:  // INC D
            w_str = __decodeInc(w_id, ai_idx, ai_exec);
            break;

        case 0x0B:  // DEC R
        case 0x05:  // DEC D
            w_str = __decodeDec(w_id, ai_idx, ai_exec);
            break;

        default:
            // Mise à jour de PC
            if (ai_exec) {
                m_pc = m_pc + 1;
            }
            break;
    }

    return w_str;
}

std::string			Cpu::__decodeNop(bool ai_exec)
{
    std::string		w_str = "";

    w_str = "NOP";

    if (ai_exec)
    {
        m_pc = m_pc + 1;
    }

    return w_str;
}

std::string			Cpu::__decodeLoad8bits(std::uint8_t ai_id, std::uint16_t ai_idx, bool ai_exec)
{
    // Variable locale
    std::string		w_str;
    std::string		w_sReg;
    std::string		w_sReg2;
    std::uint8_t 	w_data8bits         = 0;
    std::uint16_t   w_data16bits        = 0;
    std::uint8_t 	w_register          = 0;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint8_t*	wp_register8bits2   = NULL;
    std::uint16_t*	wp_register16bits   = NULL;
    std::uint16_t*	wp_register16bits2  = NULL;
    std::uint8_t    w_id                = 0;


    // Récupération de l'id de l'opcode
    w_id = ai_id;


    // Gestion de l'opcodes 0XE2 (LD (C),A)
    if (w_id == 0xE2)
    {
        // Réalise la même opération que 0xE0 (LD (FF00+N),A)
        w_id = 0xE0;
    }

    // Gestion de l'opcode 0xF2 (LD A,(C))
    if (w_id == 0xF2)
    {
        // Réalise la même opération que 0xF0 (LD A,(FF00+N))
        w_id = 0xF0;
    }


    if (w_id == 0x06)    // LD D,N
    {
        // Récupération du registre
        w_register = ( mp_mpu->getMemVal(ai_idx) & 0x38 ) >> 3;

        // Récupération de la valeur à charger dans le registre
        w_data8bits = mp_mpu->getMemVal(ai_idx + 1);

        // Chargement de la valeur dans le registre
        _decodeRegister8Bits(w_register, w_sReg, wp_register8bits, wp_register16bits);

        w_str = "LOAD " + w_sReg + "," + std::to_string(w_data8bits);
    }
    else if (w_id == 0x02)   // LD (R),A
    {
        // Récupération du registre
        w_register = mp_mpu->getMemVal(ai_idx) >> 4;

        wp_register16bits = _decodeRegister16Bits(w_register, w_id, w_sReg);

        w_str = "LOAD " + w_sReg + ",A";
    }
    else if (w_id == 0x0A)   // LD A,(R)
    {
        // Récupération du registre
        w_register = mp_mpu->getMemVal(ai_idx) >> 4;

        wp_register16bits = _decodeRegister16Bits(w_register, w_id, w_sReg);

        w_str = "LOAD A," + w_sReg;
    }
    else if (w_id == 0x22)  // LDI (HL),A
    {
        wp_register16bits = &m_registers.s16bits.hl;
        w_sReg = "(HL)";
        w_str = "LOADI " + w_sReg + ",A";
    }
    else if (w_id == 0x2A)  // LDI A,(HL)
    {
        wp_register16bits = &m_registers.s16bits.hl;
        w_sReg = "(HL)";
        w_str = "LOADI A," + w_sReg;
    }
    else if (w_id == 0x32)  // LDD (HL),A
    {
        wp_register16bits = &m_registers.s16bits.hl;
        w_sReg = "(HL)";
        w_str = "LOADD " + w_sReg + ",A";
    }
    else if (w_id == 0x3A)  // LDD A,(HL)
    {
        wp_register16bits = &m_registers.s16bits.hl;
        w_sReg = "(HL)";
        w_str = "LOADD A," + w_sReg;
    }
    else if (w_id == 0x40)  // LD D,D
    {
        // Récupération du premier registre
        w_register = ( mp_mpu->getMemVal(ai_idx) >> 3 ) & 0x07;
        _decodeRegister8Bits(w_register, w_sReg, wp_register8bits, wp_register16bits);

        // Récupération du deuxième registre
        w_register = mp_mpu->getMemVal(ai_idx) & 0x07;
        _decodeRegister8Bits(w_register, w_sReg2, wp_register8bits2, wp_register16bits2);

        w_str = "LOAD " + w_sReg +"," + w_sReg2;
    }
    else if (w_id == 0xE0)   // LD (FF00+N),A
    {
        // Récupération de l'offset N
        w_data8bits = mp_mpu->getMemVal(ai_idx + 1);

        // Registre 8 bits à utiliser
        wp_register8bits = &m_registers.s8bits.a;

        w_str = "LOAD ($FF00+" + std::to_string(w_data8bits) + "),A";
    }
    else if (w_id == 0xF0)  // LD A,(FF00+N)
    {
        // Récupération de l'offset N
        w_data8bits = mp_mpu->getMemVal(ai_idx + 1);

        // Registre 8 bits à utiliser
        wp_register8bits = &m_registers.s8bits.a;

        w_str = "LOAD A,($FF00+" + std::to_string(w_data8bits) + ")";
    }
    else if (w_id == 0xEA)  // LD (N),A
    {
        // Récupération de l'adresse d'écriture
        w_data16bits = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);

        // Registre 8 bits à utiliser
        wp_register8bits = &m_registers.s8bits.a;

        w_str = "LOAD (" + std::to_string(w_data16bits) + "),A";
    }
    else if (w_id == 0xFA)  // LD A,(N)
    {
        // Récupération de l'adresse de lecture
        w_data16bits = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);

        // Registre 8 bits à utiliser
        wp_register8bits = &m_registers.s8bits.a;

        w_str = "LOAD A,(" + std::to_string(w_data16bits) + ")";
    }


    // EXECUTION DE L'INSTRUCTION
    // **************************
    if (ai_exec && (wp_register8bits != NULL || wp_register16bits != NULL))
    {
        if (w_id == 0x06)    // LD D,N
        {
            if (w_register == 6)
            {
                // On stocke la valeur en mémoire à l'adresse contenue par HL
                mp_mpu->setMemVal(*wp_register16bits, w_data8bits);
            }
            else
            {
                // Mise à jour de la valeur du registre
                *wp_register8bits = w_data8bits;
            }

            // Mise à jour de PC
            m_pc += 2;
        }
        else if (w_id == 0x02)  // LD (R),A
        {
            // On stocke le contenu du registre A en mémoire à l'adresse contenue par wp_register16bits
            mp_mpu->setMemVal(*wp_register16bits, m_registers.s8bits.a);

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0x0A)  // LD A,(R)
        {
            // On stocke le contenu de la mémoire à l'adresse contenue par wp_register16bits dans A
            m_registers.s8bits.a = mp_mpu->getMemVal(*wp_register16bits);

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0x22)  // LDI (HL),A
        {
            // On stocke le contenu du registre A en mémoire à l'adresse contenue par HL
            mp_mpu->setMemVal(*wp_register16bits, m_registers.s8bits.a);

            // On incrémente HL
            *wp_register16bits += 1;

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0x2A)  // LDI A,(HL)
        {
            // On stocke le contenu de la mémoire à l'adresse contenue par wp_register16bits dans A
            m_registers.s8bits.a = mp_mpu->getMemVal(*wp_register16bits);

            // On incrémente HL
            *wp_register16bits += 1;

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0x32)  // LDD (HL),A
        {
            // On stocke le contenu du registre A en mémoire à l'adresse contenue par HL
            mp_mpu->setMemVal(*wp_register16bits, m_registers.s8bits.a);

            // On décrémente HL
            *wp_register16bits -= 1;

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0x3A)  // LDD A,(HL)
        {
            // On stocke le contenu de la mémoire à l'adresse contenue par wp_register16bits dans A
            m_registers.s8bits.a = mp_mpu->getMemVal(*wp_register16bits);

            // On décrémente HL
            *wp_register16bits -= 1;

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0x40)  // LD D,D
        {
            if (w_sReg == "(HL)")
            {
                // On stocke le contenu du registre wp_register8bits2 en mémoire à l'adresse contenue par HL (wp_register16bits)
                mp_mpu->setMemVal(*wp_register16bits, *wp_register8bits2);
            }
            else if (w_sReg2 == "(HL)")
            {
                // On stocke le contenu de la mémoire à l'adresse contenue par HL (wp_register16bits2) dans wp_register8bits
                *wp_register8bits = mp_mpu->getMemVal(*wp_register16bits2);
            }
            else
            {
                // On charge wp_register8bits2 dans wp_register8bits
                *wp_register8bits = *wp_register8bits2;
            }

            // Mise à jour de PC
            m_pc += 1;
        }
        else if (w_id == 0xE0)   // LD (FF00+N),A
        {
            // On stocke le contenu du registre A dans la mémoire à l'adresse $FF00+N
            mp_mpu->setMemVal((0xFF00 + w_data8bits), *wp_register8bits);

            // Mise à jour de PC
            m_pc += 2;
        }
        else if (w_id == 0xF0)  // LD A,(FF00+N)
        {
            // On stocke dans A le contenu de la mémoire à l'adresse $FF00+N
            *wp_register8bits = mp_mpu->getMemVal(0xFF00 + w_data8bits);

            // Mise à jour de PC
            m_pc += 2;
        }
        else if (w_id == 0xEA)  // LD (N),A
        {
            // On stocke le contenu de A à l'adresse mémoire (N)
            mp_mpu->setMemVal(w_data16bits, *wp_register8bits);

            // Mise à jour de PC
            m_pc += 3;
        }
        else if (w_id == 0xFA)  // LD A,(N)
        {
            // On stocke dans A le contenu de la mémoire à l'adresse (N)
            *wp_register8bits = mp_mpu->getMemVal(w_data16bits);

            // Mise à jour de PC
            m_pc += 3;
        }
    }
    else
    {
        std::cout << "ERREUR : Pointeur NULL" << std::endl;
        exit(-1);
    }

    return w_str;
}

std::string Cpu::__decodeLoad16bits(std::uint8_t ai_id, std::uint16_t ai_idx, bool ai_exec)
{
    // Variable locale
    std::string		w_str;
    std::int8_t 	w_data8bits         = 0;
    std::uint16_t 	w_data16bits        = 0;
    std::uint16_t*	wp_register16bits   = NULL;
    std::uint8_t    w_register          = 0;
    std::uint8_t    w_id                = 0;

    // Récupération de l'id de l'opcode
    w_id = ai_id;

    w_str = "LOAD ";

    // Récupération du registre à charger
    if (w_id == 0x08)
    {
        // Récupération de la valeur à charger dans le registre
        w_data16bits = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);

        wp_register16bits = &m_sp;
        w_str += "(" + std::to_string(w_data16bits) + "),SP";
    }
    else if (w_id == 0xF8)
    {
        // Récupération de la valeur 8 bits signés
        w_data8bits = (mp_mpu->getMemVal(ai_idx + 1));

        w_str += "HL,SP+" + std::to_string(w_data8bits);
    }
    else if (w_id == 0xF9)
    {
        w_str += "SP,HL";
    }
    else
    {
        // Récupération de la valeur à charger dans le registre
        w_data16bits = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);

        // On récupère l'indice du registre à utiliser
        w_register = ( mp_mpu->getMemVal(ai_idx) & 0x30 ) >> 4;

        wp_register16bits = _decodeRegister16Bits(w_register, w_id, w_str);
    }

    // EXECUTION DE L'INSTRUCTION
    // **************************
    if (ai_exec) // && (wp_register16bits != NULL)
    {
        if (w_id == 0x08)
        {
            // On stocke le LSW de la valeur du registre à l'adresse fournie en donnée
            mp_mpu->setMemVal(w_data16bits, ((*wp_register16bits)&0x00FF));

            // On stocke le MSW de la valeur du registre à l'adresse fournie en donnée + 1
            mp_mpu->setMemVal(w_data16bits+1, (((*wp_register16bits)>>8)&0x00FF));

            // Mise à jour de PC
            m_pc += 3;
        }
        else if (w_id == 0xF8)
        {
            // On stocke dans HL le résultat de l'addition de SP par la valeur 8 bits fournie
            m_registers.s16bits.hl = m_sp + w_data8bits;

            // Reset des flags Z et N
            m_registers.sFlags.z = 0;
            m_registers.sFlags.n = 0;

            // Gestion du flag C
            if ((static_cast<int32_t>(m_sp) + static_cast<int32_t>(w_data8bits) > 0xFFFF) || (static_cast<int32_t>(m_sp) + static_cast<int32_t>(w_data8bits) < 0))
            {
                m_registers.sFlags.c = 1;
            }
            else
            {
                m_registers.sFlags.c = 0;
            }

            // Gestion du flag H
            if ((((m_sp & 0x0FFF) + (w_data8bits & 0x0FFF)) & 0x1000))
            {
                m_registers.sFlags.h = 1;
            }
            else
            {
                m_registers.sFlags.h = 0;
            }

            // Mise à jour de PC
            m_pc += 2;
        }
        else if (w_id == 0xF9)
        {
            // On stocke le contenu de HL dans SP
            m_sp = m_registers.s16bits.hl;

            // Mise à jour de PC
            m_pc += 1;
        }
        else
        {
            // On stocke la valeur dans le registre
            *wp_register16bits = w_data16bits;

            // Mise à jour de PC
            m_pc += 3;
        }
    }

    return w_str;
}

std::string	Cpu::__decodeJump(std::uint8_t ai_id, std::uint16_t ai_idx, bool ai_exec)
{
    std::string		w_str = "JP ";
    std::uint16_t	w_pos = 0;
    std::uint8_t	w_mnemo = 0;
    bool			w_test = true;
    std::uint8_t	w_size = 1;

    // DECODAGE DE LA COMMANDE JUMP
    // ****************************
    w_mnemo = (mp_mpu->getMemVal(ai_idx) & 0x18) >> 3;

    switch(ai_id)
    {
    case 0xE9:  // JP HL
        w_pos = getRegisterHL();
        w_str += "HL";
        w_size = 1;
        break;
    case 0xC2:  // JP F,N
        w_pos = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);
        switch (w_mnemo)
        {
        case 0:
            w_str += "NZ,";
            w_test = (getFlagZ() == 0);
            break;
        case 1:
            w_str += "Z,";
            w_test = (getFlagZ() == 1);
            break;
        case 2:
            w_str += "NC,";
            w_test = (getFlagC() == 0);
            break;
        case 3:
            w_str += "C,";
            w_test = (getFlagC() == 1);
            break;
        }
        w_str += std::to_string(w_pos);
        w_size = 3;
        break;
    case 0xC3:  // JP N
        w_pos = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);
        w_str += std::to_string(w_pos);
        w_size = 3;
        break;
    }

    // EXECUTION DE LA COMMANDE
    // ************************
    if (ai_exec)
    {
        if (w_test)
        {
            m_pc = w_pos;
        }
        else
        {
            m_pc += w_size;
        }
    }

    return w_str;
}

std::string Cpu::__decodeInc(uint8_t ai_id, uint16_t ai_idx, bool ai_exec) {
    std::string w_str            = "INC ";
    uint8_t w_register           = 0u;
    uint16_t * wp_register16bits = NULL;
    uint8_t * wp_register8bits   = NULL;

    switch (ai_id) {
        case 0x03: // INC R sur registre 16b
            // Récupération du registre
            w_register = (mp_mpu->getMemVal(ai_idx) >> 4);
            wp_register16bits = _decodeRegister16Bits(w_register, ai_id, w_str);

        case 0x4: // INC D
            // Récupération du registre
            w_register = ((mp_mpu->getMemVal(ai_idx) & 0x38) >> 3);
            _decodeRegister8Bits(w_register, w_str, wp_register8bits, wp_register16bits);

            break;
    }
    if (ai_exec) {
        switch (ai_id) {
        case 0x03:
            //Registre 16 bits, pas de modifications des flags
            ++(*wp_register16bits);
            break;

        case 0x04:
            // Registre 8 bits, prendre en compte les modifications des flags
            switch(w_register) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
                // Gestion du Half-Carry
                if (((*wp_register8bits) & 0x0F) == 0x0F) {
                    m_registers.sFlags.h = 1;
                } else {
                    m_registers.sFlags.h = 0;
                }
                // Incrémentation
                ++(*wp_register8bits);
                // Gestion du zéro
                if ((*wp_register8bits) == 0) {
                    m_registers.sFlags.z = 1;
                } else {
                    m_registers.sFlags.z = 0;
                }
                // Reset du substract
                m_registers.sFlags.n = 0;
                // Flag carry non modifié
                break;
            case 6:
                uint8_t w_val = mp_mpu->getMemVal(*wp_register16bits);
                if ((w_val & 0x0F) == 0x0F) {
                    m_registers.sFlags.h = 1;
                } else {
                    m_registers.sFlags.h = 0;
                }
                // Incrémentation
                mp_mpu->setMemVal(*wp_register16bits, ++w_val);
                //Gestion du Zéro
                if (w_val == 0) {
                    m_registers.sFlags.z = 1;
                } else {
                    m_registers.sFlags.z = 0;
                }
                // Reset du substract
                m_registers.sFlags.n = 0;
                // Flag carry non modifié
                break;
            }
            break;
        }
        ++m_pc;
    }
    return w_str;
}

std::string Cpu::__decodeDec(uint8_t ai_id, uint16_t ai_idx, bool ai_exec) {
    std::string w_str            = "DEC ";
    uint8_t w_register           = 0u;
    uint16_t * wp_register16bits = NULL;
    uint8_t * wp_register8bits   = NULL;
    uint16_t w_pos               = 0u;

    switch (ai_id) {
        case 0x0B: // DEC R sur registre 16b
            // Récupération du registre
            w_register = (mp_mpu->getMemVal(ai_idx) >> 4);
            wp_register16bits = _decodeRegister16Bits(w_register, ai_id, w_str);

        case 0x5: // DEC D
            // Récupération du registre
            w_register = ((mp_mpu->getMemVal(ai_idx) & 0x38) >> 3);
            _decodeRegister8Bits(w_register, w_str, wp_register8bits, wp_register16bits);

            break;
    }

    if (ai_exec) {
        switch (ai_id) {
        case 0x0B:
            //Registre 16 bits, pas de modifications des flags
            --(*wp_register16bits);
            break;

        case 0x05:
            // Registre 8 bits, prendre en compte les modifications des flags
            switch(w_register) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
                // Gestion du Half-Carry
                if (((*wp_register8bits) & 0x0F) == 0x00) {
                    m_registers.sFlags.h = 1;
                } else {
                    m_registers.sFlags.h = 0;
                }
                // Décrémentation
                --(*wp_register8bits);
                // Gestion du zéro
                if ((*wp_register8bits) == 0) {
                    m_registers.sFlags.z = 1;
                } else {
                    m_registers.sFlags.z = 0;
                }
                // Set du substract
                m_registers.sFlags.n = 1;
                // Flag carry non modifié
                break;
            case 6:
                uint8_t w_val = mp_mpu->getMemVal(w_pos);
                if ((w_val & 0x0F) == 0x00) {
                    m_registers.sFlags.h = 1;
                } else {
                    m_registers.sFlags.h = 0;
                }
                // Décrémentation
                mp_mpu->setMemVal(w_pos, --w_val);
                //Gestion du Zéro
                if (w_val == 0) {
                    m_registers.sFlags.z = 1;
                } else {
                    m_registers.sFlags.z = 0;
                }
                // Set du substract
                m_registers.sFlags.n = 1;
                // Flag carry non modifié
                break;
            }
            break;
        }
        ++m_pc;
    }
    return w_str;
}


void Cpu::_decodeRegister8Bits(std::uint8_t ai_registerMask, std::string &ai_sReg, std::uint8_t* &aop_register8bits, std::uint16_t* &aop_register16bits)
{
    switch (ai_registerMask)
    {
        case 0:
            aop_register8bits = &m_registers.s8bits.b;
            ai_sReg = "B";
            break;

        case 1:
            aop_register8bits = &m_registers.s8bits.c;
            ai_sReg = "C";
            break;

        case 2:
            aop_register8bits = &m_registers.s8bits.d;
            ai_sReg = "D";
            break;

        case 3:
            aop_register8bits = &m_registers.s8bits.e;
            ai_sReg = "E";
            break;

        case 4:
            aop_register8bits = &m_registers.s8bits.h;
            ai_sReg = "H";
            break;

        case 5:
            aop_register8bits = &m_registers.s8bits.l;
            ai_sReg = "L";
            break;

        case 6:
            aop_register16bits = &m_registers.s16bits.hl;
            ai_sReg = "(HL)";
            break;

        case 7:
            aop_register8bits = &m_registers.s8bits.a;
            ai_sReg = "A";
            break;

        default:
            std::cout << "ERREUR : Registre 8 bits inconnu" << std::endl;
            break;
    }
}


std::uint16_t* Cpu::_decodeRegister16Bits(std::uint8_t ai_registerMask, std::uint8_t ai_opcodeId, std::string &ai_sReg)
{
    std::uint16_t* wp_registers16bit = NULL;

    switch (ai_registerMask) {
        case 0:
            ai_sReg += "BC";
            wp_registers16bit = &m_registers.s16bits.bc;
            break;

        case 1:
            ai_sReg += "DE";
            wp_registers16bit = &m_registers.s16bits.de;
            break;

        case 2:
            ai_sReg += "HL";
            wp_registers16bit = &m_registers.s16bits.hl;
            break;

        case 3:
            // Renvoie AF uniquement dans le cas d'un POP ou d'un PUSH, SP sinon
            if (ai_opcodeId == 0xC1 || ai_opcodeId == 0xC5)
            {
                ai_sReg += "AF";
                wp_registers16bit = &m_registers.s16bits.af;
            }
            else
            {
                ai_sReg += "SP";
                wp_registers16bit = &m_sp;
            }
        break;

    default:
        std::cout << "ERREUR : Registre 16 bits inconnu" << std::endl;
        break;
    }

    return wp_registers16bit;
}
