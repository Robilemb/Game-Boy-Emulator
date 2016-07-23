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


std::string			Cpu::showInstruction(std::uint16_t ai_idx)
{
	std::string		w_str = "";

    w_str = decodeInstr(mp_mpu->getMemVal(ai_idx), false);

	return w_str;
}

std::uint8_t		Cpu::showInstructionId(std::uint16_t ai_idx)
{
    std::uint8_t 	w_id = decodeOpcode(mp_mpu->getMemVal(ai_idx));

	return w_id;
}

std::string			Cpu::showInstructionIdStr(std::uint16_t ai_idx)
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
            w_str = __decodeLoad8bits(w_id, ai_idx, ai_exec);
        	break;

        case 0x08:  // LD (N),SP
        case 0x01:  // LD R,N
            w_str = __decodeLoad16bits(w_id, ai_idx, ai_exec);
            break;

        case 0xC2:  // JP F,N
        case 0xC3:  // JP N
        case 0xE9:  // JP HL
            w_str = __decodeJump(w_id, ai_idx, ai_exec);
        	break;

        default:
            // Mise à jour de PC
            m_pc = m_pc + 1;
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

std::string			Cpu::__decodeLoad8bits(std::uint8_t w_id, std::uint16_t ai_idx, bool ai_exec)
{
    // Variable locale
    std::string		w_str;
    std::string		w_sReg;
    std::uint8_t 	w_data8bits         = 0;
    std::uint8_t 	w_register          = 0;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint16_t*	wp_register16bits   = NULL;

    w_str = "LOAD ";

    if (w_id == 0x06)    // LD D,N
    {
        // Récupération du registre
        w_register = ( mp_mpu->getMemVal(ai_idx) & 0x38 ) >> 3;

        // Récupération de la valeur à charger dans le registre
        w_data8bits = mp_mpu->getMemVal(ai_idx + 1);

        // Chargement de la valeur dans le registre
        if (w_register == 0)
        {
            wp_register8bits = &m_registers.s8bits.b;
            w_sReg = "B";
        }
        else if (w_register == 1)
        {
            wp_register8bits = &m_registers.s8bits.c;
            w_sReg = "C";
        }
        else if (w_register == 2)
        {
            wp_register8bits = &m_registers.s8bits.d;
            w_sReg = "D";
        }
        else if (w_register == 3)
        {
            wp_register8bits = &m_registers.s8bits.e;
            w_sReg = "E";
        }
        else if (w_register == 4)
        {
            wp_register8bits = &m_registers.s8bits.h;
            w_sReg = "H";
        }
        else if (w_register == 5)
        {
            wp_register8bits = &m_registers.s8bits.l;
            w_sReg = "L";
        }
        else if (w_register == 6)
        {
            wp_register16bits = &m_registers.s16bits.hl;
            w_sReg = "(HL)";
        }
        else if (w_register == 7)
        {
            wp_register8bits = &m_registers.s8bits.a;
            w_sReg = "A";
        }

        w_str = "LOAD " + w_sReg + "," + std::to_string(w_data8bits);
    }
    else if (w_id == 0x02)   // LD (R),A
    {
        // Récupération du registre
        w_register = mp_mpu->getMemVal(ai_idx) >> 4;

        if (w_register == 0)
        {
            wp_register16bits = &m_registers.s16bits.bc;
            w_sReg = "(BC)";
        }
        else if (w_register == 1)
        {
            wp_register16bits = &m_registers.s16bits.de;
            w_sReg = "(DE)";
        }

        w_str = "LOAD " + w_sReg + ",A";
    }
    else if (w_id == 0x0A)   // LD A,(R)
    {
        // Récupération du registre
        w_register = mp_mpu->getMemVal(ai_idx) >> 4;

        if (w_register == 0)
        {
            wp_register16bits = &m_registers.s16bits.bc;
            w_sReg = "(BC)";
        }
        else if (w_register == 1)
        {
            wp_register16bits = &m_registers.s16bits.de;
            w_sReg = "(DE)";
        }

        w_str = "LOAD A," + w_sReg;
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
    }

    return w_str;
}

std::string			Cpu::__decodeLoad16bits(std::uint8_t ai_id, std::uint16_t ai_idx, bool ai_exec)
{
    // Variable locale
    std::string		w_str;
    std::uint16_t 	w_data16bits        = 0;
    std::uint16_t*	wp_register16bits   = NULL;
    std::uint8_t    w_register          = 0;

    // Récupération de la valeur à charger dans le registre
    w_data16bits = (mp_mpu->getMemVal(ai_idx + 2) * 0x100) + mp_mpu->getMemVal(ai_idx + 1);

    w_str = "LOAD ";

    // Récupération du registre à charger
    if (ai_id == 0x08)
    {
        wp_register16bits = &m_sp;
        w_str += "(" + std::to_string(w_data16bits) + "),SP";
    }
    else
    {
        // On récupère l'indice du registre à utiliser
        w_register = ( mp_mpu->getMemVal(ai_idx) & 0x30 ) >> 4;

        if (w_register == 0)
        {
            wp_register16bits = &m_registers.s16bits.bc;
            w_str += "BC," + std::to_string(w_data16bits);
        }
        else if (w_register == 1)
        {
            wp_register16bits = &m_registers.s16bits.de;
            w_str += "DE," + std::to_string(w_data16bits);
        }
        else if (w_register == 2)
        {
            wp_register16bits = &m_registers.s16bits.hl;
            w_str += "HL," + std::to_string(w_data16bits);
        }
        else if (w_register == 3)
        {
            wp_register16bits = &m_sp;
            w_str += "SP," + std::to_string(w_data16bits);
        }
    }

    // EXECUTION DE L'INSTRUCTION
    // **************************
    if (ai_exec && (wp_register16bits != NULL))
    {
        if (ai_id == 0x08)
        {
            // On stocke le LSW de la valeur du registre à l'adresse fournie en donnée
            mp_mpu->setMemVal(w_data16bits, ((*wp_register16bits)&0x00FF));

            // On stocke le MSW de la valeur du registre à l'adresse fournie en donnée + 1
            mp_mpu->setMemVal(w_data16bits+1, (((*wp_register16bits)>>8)&0x00FF));
        }
        else
        {
            // On stocke la valeur dans le registre
            *wp_register16bits = w_data16bits;
        }

        // Mise à jour de PC
        m_pc += 3;
    }

    return w_str;
}

std::string			Cpu::__decodeJump(std::uint8_t ai_id, std::uint16_t ai_idx, bool ai_exec)
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
