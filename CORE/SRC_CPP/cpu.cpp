#include <string>
#include "CORE/INCLUDE/cpu.h"


// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Cpu::Cpu(Mpu* ai_mpu)
{
    // Initialisation des données membres
    m_opcodeIdx = 0u;

    // Récupération du pointeur vers la mémoire
    mp_mpu = ai_mpu;

    // Initialisation des registres
    initRegisters();

    // Initialisation des masques et identifiants des opcodes
    _initOpcodesDesc();
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

void Cpu::_initOpcodesDesc()
{
    // Initialisation des masques et identifiants des opcodes 8 bits
    m_opcodesDesc.masque8bits[0]  = 0xFF; m_opcodesDesc.id8bits[0]  = 0x00; m_opcodesDesc.execute8bits[0]   = &Cpu::_nop;                 // NOP
    m_opcodesDesc.masque8bits[1]  = 0xFF; m_opcodesDesc.id8bits[1]  = 0x08; m_opcodesDesc.execute8bits[1]   = &Cpu::_ld_n_sp;             // LD (N),SP
    m_opcodesDesc.masque8bits[2]  = 0xCF; m_opcodesDesc.id8bits[2]  = 0x01; m_opcodesDesc.execute8bits[2]   = &Cpu::_ld_r_n;              // LD R,N
    m_opcodesDesc.masque8bits[3]  = 0xCF; m_opcodesDesc.id8bits[3]  = 0x09; m_opcodesDesc.execute8bits[3]   = &Cpu::_add_hl_r;            // ADD HL,R
    m_opcodesDesc.masque8bits[4]  = 0xEF; m_opcodesDesc.id8bits[4]  = 0x02; m_opcodesDesc.execute8bits[4]   = &Cpu::_ld_r_a;              // LD (R),A
    m_opcodesDesc.masque8bits[5]  = 0xEF; m_opcodesDesc.id8bits[5]  = 0x0A; m_opcodesDesc.execute8bits[5]   = &Cpu::_ld_a_r;              // LD A,(R)
    m_opcodesDesc.masque8bits[6]  = 0xCF; m_opcodesDesc.id8bits[6]  = 0x03; m_opcodesDesc.execute8bits[6]   = &Cpu::_inc_r;               // INC R
    m_opcodesDesc.masque8bits[7]  = 0xCF; m_opcodesDesc.id8bits[7]  = 0x0B; m_opcodesDesc.execute8bits[7]   = &Cpu::_dec_r;               // DEC R
    m_opcodesDesc.masque8bits[8]  = 0xC7; m_opcodesDesc.id8bits[8]  = 0x04; m_opcodesDesc.execute8bits[8]   = &Cpu::_inc_d;               // INC D
    m_opcodesDesc.masque8bits[9]  = 0xC7; m_opcodesDesc.id8bits[9]  = 0x05; m_opcodesDesc.execute8bits[9]   = &Cpu::_dec_d;               // DEC D
    m_opcodesDesc.masque8bits[10] = 0xC7; m_opcodesDesc.id8bits[10] = 0x06; m_opcodesDesc.execute8bits[10]  = &Cpu::_ld_d_n;              // LD D,N
    m_opcodesDesc.masque8bits[11] = 0xF7; m_opcodesDesc.id8bits[11] = 0x07; m_opcodesDesc.execute8bits[11]  = &Cpu::_rdca;                // RdCA
    m_opcodesDesc.masque8bits[12] = 0xF7; m_opcodesDesc.id8bits[12] = 0x17; m_opcodesDesc.execute8bits[12]  = &Cpu::_rda;                 // RdA
    m_opcodesDesc.masque8bits[13] = 0xFF; m_opcodesDesc.id8bits[13] = 0x10; m_opcodesDesc.execute8bits[13]  = &Cpu::_stop;                // STOP
    m_opcodesDesc.masque8bits[14] = 0xFF; m_opcodesDesc.id8bits[14] = 0x18; m_opcodesDesc.execute8bits[14]  = &Cpu::_jr_n;                // JR N
    m_opcodesDesc.masque8bits[15] = 0xE7; m_opcodesDesc.id8bits[15] = 0x20; m_opcodesDesc.execute8bits[15]  = &Cpu::_jr_f_n;              // JR F,N
    m_opcodesDesc.masque8bits[16] = 0xFF; m_opcodesDesc.id8bits[16] = 0x22; m_opcodesDesc.execute8bits[16]  = &Cpu::_ldi_hl_a;            // LDI (HL),A
    m_opcodesDesc.masque8bits[17] = 0xFF; m_opcodesDesc.id8bits[17] = 0x2A; m_opcodesDesc.execute8bits[17]  = &Cpu::_ldi_a_hl;            // LDI A,(HL)
    m_opcodesDesc.masque8bits[18] = 0xFF; m_opcodesDesc.id8bits[18] = 0x32; m_opcodesDesc.execute8bits[18]  = &Cpu::_ldd_hl_a;            // LDD (HL),A
    m_opcodesDesc.masque8bits[19] = 0xFF; m_opcodesDesc.id8bits[19] = 0x3A; m_opcodesDesc.execute8bits[19]  = &Cpu::_ldd_a_hl;            // LDD A,(HL)
    m_opcodesDesc.masque8bits[20] = 0xFF; m_opcodesDesc.id8bits[20] = 0x27; m_opcodesDesc.execute8bits[20]  = &Cpu::_daa;                 // DAA
    m_opcodesDesc.masque8bits[21] = 0xFF; m_opcodesDesc.id8bits[21] = 0x2F; m_opcodesDesc.execute8bits[21]  = &Cpu::_cpl;                 // CPL
    m_opcodesDesc.masque8bits[22] = 0xFF; m_opcodesDesc.id8bits[22] = 0x37; m_opcodesDesc.execute8bits[22]  = &Cpu::_scf;                 // SCF
    m_opcodesDesc.masque8bits[23] = 0xFF; m_opcodesDesc.id8bits[23] = 0x3F; m_opcodesDesc.execute8bits[23]  = &Cpu::_ccf;                 // CCF
    m_opcodesDesc.masque8bits[24] = 0xC0; m_opcodesDesc.id8bits[24] = 0x40; m_opcodesDesc.execute8bits[24]  = &Cpu::_ld_d_d;              // LD D,D
    m_opcodesDesc.masque8bits[25] = 0xFF; m_opcodesDesc.id8bits[25] = 0x76; m_opcodesDesc.execute8bits[25]  = &Cpu::_halt;                // HALT
    m_opcodesDesc.masque8bits[26] = 0xC0; m_opcodesDesc.id8bits[26] = 0x80; m_opcodesDesc.execute8bits[26]  = &Cpu::_alu_a_d;             // ALU A,D
    m_opcodesDesc.masque8bits[27] = 0xC7; m_opcodesDesc.id8bits[27] = 0xC6; m_opcodesDesc.execute8bits[27]  = &Cpu::_alu_a_n;             // ALU A,N
    m_opcodesDesc.masque8bits[28] = 0xCF; m_opcodesDesc.id8bits[28] = 0xC1; m_opcodesDesc.execute8bits[28]  = &Cpu::_pop_r;               // POP R
    m_opcodesDesc.masque8bits[29] = 0xCF; m_opcodesDesc.id8bits[29] = 0xC5; m_opcodesDesc.execute8bits[29]  = &Cpu::_push_r;              // PUSH R
    m_opcodesDesc.masque8bits[30] = 0xC7; m_opcodesDesc.id8bits[30] = 0xC7; m_opcodesDesc.execute8bits[30]  = &Cpu::_rst_n;               // RST N
    m_opcodesDesc.masque8bits[31] = 0xE7; m_opcodesDesc.id8bits[31] = 0xC0; m_opcodesDesc.execute8bits[31]  = &Cpu::_ret_f;               // RET F
    m_opcodesDesc.masque8bits[32] = 0xFF; m_opcodesDesc.id8bits[32] = 0xC9; m_opcodesDesc.execute8bits[32]  = &Cpu::_ret;                 // RET
    m_opcodesDesc.masque8bits[33] = 0xFF; m_opcodesDesc.id8bits[33] = 0xD9; m_opcodesDesc.execute8bits[33]  = &Cpu::_reti;                // RETI
    m_opcodesDesc.masque8bits[34] = 0xE7; m_opcodesDesc.id8bits[34] = 0xC2; m_opcodesDesc.execute8bits[34]  = &Cpu::_jp_f_n;              // JP F,N
    m_opcodesDesc.masque8bits[35] = 0xFF; m_opcodesDesc.id8bits[35] = 0xC3; m_opcodesDesc.execute8bits[35]  = &Cpu::_jp_n;                // JP N
    m_opcodesDesc.masque8bits[36] = 0xE7; m_opcodesDesc.id8bits[36] = 0xC4; m_opcodesDesc.execute8bits[36]  = &Cpu::_call_f_n;            // CALL F,N
    m_opcodesDesc.masque8bits[37] = 0xFF; m_opcodesDesc.id8bits[37] = 0xCD; m_opcodesDesc.execute8bits[37]  = &Cpu::_call_n;              // CALL N
    m_opcodesDesc.masque8bits[38] = 0xFF; m_opcodesDesc.id8bits[38] = 0xE8; m_opcodesDesc.execute8bits[38]  = &Cpu::_add_sp_n;            // ADD SP,N
    m_opcodesDesc.masque8bits[39] = 0xFF; m_opcodesDesc.id8bits[39] = 0xF8; m_opcodesDesc.execute8bits[39]  = &Cpu::_ld_hl_sp_plus_n;     // LD HL,SP+N
    m_opcodesDesc.masque8bits[40] = 0xFF; m_opcodesDesc.id8bits[40] = 0xE0; m_opcodesDesc.execute8bits[40]  = &Cpu::_ld_ff00_plus_n_a;    // LD (FF00+N),A
    m_opcodesDesc.masque8bits[41] = 0xFF; m_opcodesDesc.id8bits[41] = 0xF0; m_opcodesDesc.execute8bits[41]  = &Cpu::_ld_a_ff00_plus_n;    // LD A,(FF00+N)
    m_opcodesDesc.masque8bits[42] = 0xFF; m_opcodesDesc.id8bits[42] = 0xE2; m_opcodesDesc.execute8bits[42]  = &Cpu::_ld_c_a;              // LD (C),A
    m_opcodesDesc.masque8bits[43] = 0xFF; m_opcodesDesc.id8bits[43] = 0xF2; m_opcodesDesc.execute8bits[43]  = &Cpu::_ld_a_c;              // LD A,(C)
    m_opcodesDesc.masque8bits[44] = 0xFF; m_opcodesDesc.id8bits[44] = 0xEA; m_opcodesDesc.execute8bits[44]  = &Cpu::_ld_n_a;              // LD (N),A
    m_opcodesDesc.masque8bits[45] = 0xFF; m_opcodesDesc.id8bits[45] = 0xFA; m_opcodesDesc.execute8bits[45]  = &Cpu::_ld_a_n;              // LD A,(N)
    m_opcodesDesc.masque8bits[46] = 0xFF; m_opcodesDesc.id8bits[46] = 0xE9; m_opcodesDesc.execute8bits[46]  = &Cpu::_jp_hl;               // JP HL
    m_opcodesDesc.masque8bits[47] = 0xFF; m_opcodesDesc.id8bits[47] = 0xF9; m_opcodesDesc.execute8bits[47]  = &Cpu::_ld_sp_hl;            // LD SP,HL
    m_opcodesDesc.masque8bits[48] = 0xFF; m_opcodesDesc.id8bits[48] = 0xF3; m_opcodesDesc.execute8bits[48]  = &Cpu::_di;                  // DI
    m_opcodesDesc.masque8bits[49] = 0xFF; m_opcodesDesc.id8bits[49] = 0xFB; m_opcodesDesc.execute8bits[49]  = &Cpu::_ei;                  // EI
    m_opcodesDesc.masque8bits[50] = 0xFF; m_opcodesDesc.id8bits[50] = 0xCB; m_opcodesDesc.execute8bits[50]  = &Cpu::_16b_opcode;          // 16b opcode

    // Initialisation des masques et identifiants des LSW des opcodes 16 bits
    m_opcodesDesc.masque16bitsLSW[0] = 0xF0; m_opcodesDesc.id16bitsLSW[0] = 0x00; m_opcodesDesc.execute16bits[0] = &Cpu::_rdc_d;          // RdC D
    m_opcodesDesc.masque16bitsLSW[1] = 0xF0; m_opcodesDesc.id16bitsLSW[1] = 0x10; m_opcodesDesc.execute16bits[1] = &Cpu::_rd_d;           // Rd D
    m_opcodesDesc.masque16bitsLSW[2] = 0xF0; m_opcodesDesc.id16bitsLSW[2] = 0x20; m_opcodesDesc.execute16bits[2] = &Cpu::_sda_d;          // SdA D
    m_opcodesDesc.masque16bitsLSW[3] = 0xF8; m_opcodesDesc.id16bitsLSW[3] = 0x30; m_opcodesDesc.execute16bits[3] = &Cpu::_swap_d;         // SWAP D
    m_opcodesDesc.masque16bitsLSW[4] = 0xF8; m_opcodesDesc.id16bitsLSW[4] = 0x38; m_opcodesDesc.execute16bits[4] = &Cpu::_srl_d;          // SRL D
    m_opcodesDesc.masque16bitsLSW[5] = 0xC0; m_opcodesDesc.id16bitsLSW[5] = 0x40; m_opcodesDesc.execute16bits[5] = &Cpu::_bit_n_d;        // BIT N,D
    m_opcodesDesc.masque16bitsLSW[6] = 0xC0; m_opcodesDesc.id16bitsLSW[6] = 0x80; m_opcodesDesc.execute16bits[6] = &Cpu::_res_n_d;        // RES N,D
    m_opcodesDesc.masque16bitsLSW[7] = 0xC0; m_opcodesDesc.id16bitsLSW[7] = 0xC0; m_opcodesDesc.execute16bits[7] = &Cpu::_set_n_d;        // SET N,D
}


// ********************************************************
// EXECUTION D'UN OPCODE
// ********************************************************

void Cpu::executeOpcode(const std::uint16_t ai_opcodeIdx)
{
    std::uint16_t   w_i       = 0u;
    std::uint16_t   w_id      = 0u;
    std::uint8_t    w_opcode  = mp_mpu->getMemVal(ai_opcodeIdx);

    // Sauvegarde de l'opcode courant
    m_opcodeIdx = ai_opcodeIdx;

    // Récupération de l'ID de l'opcode à executer
    for (w_i = 0u; w_i < CPU_NB_OPCODES_8_BITS; ++w_i)
    {
        // Recherche de l'ID correspondant à l'opcode 8 bits
        w_id = (m_opcodesDesc.masque8bits[w_i] & w_opcode);

        if (w_id == m_opcodesDesc.id8bits[w_i])
        {
            // Exécution de l'opcode
            (this->*m_opcodesDesc.execute8bits[w_i])();
            break;
        }
    }

    // Gestion opcode inconnu
    if (w_i == CPU_NB_OPCODES_8_BITS)
    {
        std::cout << "Erreur : opcode inconnu" << std::endl;
        exit(-1);
    }
}


// *****************
// Fonctions privées
// *****************

bool Cpu::_decodeMnemonic(const std::uint8_t ai_mnemo)
{
    bool w_test = true;

    switch (ai_mnemo)
    {
        case 0u:
            w_test = (m_registers.sFlags.z == 0u);
            break;

        case 1u:
            w_test = (m_registers.sFlags.z == 1u);
            break;

        case 2u:
            w_test = (m_registers.sFlags.c == 0u);
            break;

        case 3u:
            w_test = (m_registers.sFlags.c == 1u);
            break;

        default:
            std::cout << "ERREUR : Mnemonique inconnu" << std::endl;
            exit(-1);
            break;
    }

    return w_test;
}

void Cpu::_decodeRegister8Bits(const std::uint8_t ai_registerMask, std::uint8_t* &aop_register8bits, std::uint16_t* &aop_register16bits)
{
    switch (ai_registerMask)
    {
        case 0u:
            aop_register8bits = &m_registers.s8bits.b;
            break;

        case 1u:
            aop_register8bits = &m_registers.s8bits.c;
            break;

        case 2u:
            aop_register8bits = &m_registers.s8bits.d;
            break;

        case 3u:
            aop_register8bits = &m_registers.s8bits.e;
            break;

        case 4u:
            aop_register8bits = &m_registers.s8bits.h;
            break;

        case 5u:
            aop_register8bits = &m_registers.s8bits.l;
            break;

        case 6u:
            aop_register16bits = &m_registers.s16bits.hl;
            break;

        case 7u:
            aop_register8bits = &m_registers.s8bits.a;
            break;

        default:
            std::cout << "ERREUR : Registre 8 bits inconnu" << std::endl;
            exit(-1);
            break;
    }
}

void Cpu::_decodeRegister16Bits(const std::uint8_t ai_registerMask, std::uint16_t* &aop_register16bits)
{
    switch (ai_registerMask)
    {
        case 0u:
            aop_register16bits = &m_registers.s16bits.bc;
            break;

        case 1u:
            aop_register16bits = &m_registers.s16bits.de;
            break;

        case 2u:
            aop_register16bits = &m_registers.s16bits.hl;
            break;

        case 3u:
            // Renvoie AF uniquement dans le cas d'un POP ou d'un PUSH, SP sinon
            if (m_opcodeIdx == 0xC1 || m_opcodeIdx == 0xC5)
            {
                aop_register16bits = &m_registers.s16bits.af;
            }
            else
            {
                aop_register16bits = &m_sp;
            }
            break;

        default:
            std::cout << "ERREUR : Registre 16 bits inconnu" << std::endl;
            exit(-1);
            break;
    }
}


// **************************
// Implémentation des opcodes
// **************************

void Cpu::_nop()
{
    // Passage à l'instruction suivante
    m_pc = m_pc + 1u;
}

void Cpu::_ld_n_sp()
{
    // Variables locales
    std::uint16_t w_address = 0u;

    // Récupération de l'adresse d'écriture
    w_address = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage des 8 bits de poids faible de SP à l'adresse fournie en entrée
    mp_mpu->setMemVal(w_address, (m_sp & 0xFF));

    // Stockage des 8 bits de poids fort de SP à l'adresse fournie en entrée + 1
    mp_mpu->setMemVal((w_address + 1u), ((m_sp >> 8u) & 0xFF));

    // Mise à jour de PC
    m_pc += 3u;
}

void Cpu::_ld_r_n()
{
    // Variables locales
    std::uint16_t 	w_data16bits        = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération de la valeur à charger dans le registre 16b
    w_data16bits = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Récupération du registre 16b
    w_registerMask = (mp_mpu->getMemVal(m_opcodeIdx) & 0x30) >> 4u;
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Stockage de la valeur dans le registre
    *wp_register16bits = w_data16bits;

    // Mise à jour de PC
    m_pc += 3u;
}

void Cpu::_add_hl_r()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;
    std::int32_t    w_sum               = 0;

    // Récupération du registre 16b
    w_registerMask = (mp_mpu->getMemVal(m_opcodeIdx) & 0x30) >> 4u;
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Additionne à HL la valeur du registre 16b
    w_sum = static_cast<std::int32_t>(m_registers.s16bits.hl) + static_cast<std::int32_t>(*wp_register16bits);

    // Gestion du flag C
    m_registers.sFlags.c = static_cast<std::uint8_t>((w_sum & 0x10000) == 0x10000);

    // Gestion du flag H
    m_registers.sFlags.h = static_cast<std::uint8_t>((((m_registers.s16bits.hl & 0x0FFF) + (*wp_register16bits & 0x0FFF)) & 0x1000) == 0x1000);

    // Gestion du flag N
    m_registers.sFlags.n = 0u;

    // Sauvegarde dans HL
    m_registers.s16bits.hl = static_cast<std::uint16_t>(w_sum & 0xFFFF);

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ld_r_a()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération du registre 16b
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) >> 4u;
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Stockage de la valeur du registre A à l'adresse contenue par wp_register16bits
    mp_mpu->setMemVal(*wp_register16bits, m_registers.s8bits.a);

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ld_a_r()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération du registre 16b
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) >> 4u;
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Stockage de la valeur à l'adresse contenue par wp_register16bits dans A
    m_registers.s8bits.a = mp_mpu->getMemVal(*wp_register16bits);

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_inc_r()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération du registre 16b
    w_registerMask = (mp_mpu->getMemVal(m_opcodeIdx) >> 4u);
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Incrémentation
    ++(*wp_register16bits);

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_dec_r()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération du registre 16b
    w_registerMask = (mp_mpu->getMemVal(m_opcodeIdx) >> 4u);
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Décrémentation
    --(*wp_register16bits);

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_inc_d()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint16_t*	wp_register16bits   = NULL;
    std::uint8_t    w_valueAtHL         = 0u;

    // Récupération du registre 8b
    w_registerMask = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u);
    _decodeRegister8Bits(w_registerMask, wp_register8bits, wp_register16bits);

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur à l'adresse contenue par HL
        w_valueAtHL = mp_mpu->getMemVal(*wp_register16bits);

        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>((w_valueAtHL & 0x0F) == 0x0F);

        // Incrémentation de la valeur à l'adresse contenue par HL
        mp_mpu->setMemVal(*wp_register16bits, ++w_valueAtHL);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(w_valueAtHL == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 1u;
    }
    else
    {
        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>((*wp_register8bits & 0x0F) == 0x0F);

        // Décrémentation du registre 8 bits
        ++(*wp_register8bits);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 1u;
    }

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_dec_d()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint16_t*	wp_register16bits   = NULL;
    std::uint8_t    w_valueAtHL         = 0u;

    // Récupération du registre 8b
    w_registerMask = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u);
    _decodeRegister8Bits(w_registerMask, wp_register8bits, wp_register16bits);

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur à l'adresse contenue par HL
        w_valueAtHL = mp_mpu->getMemVal(*wp_register16bits);

        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>(w_valueAtHL == 0u);

        // Décrémentation de la valeur à l'adresse contenue par HL
        mp_mpu->setMemVal(*wp_register16bits, --w_valueAtHL);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(w_valueAtHL == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 1u;
    }
    else
    {
        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Décrémentation du registre 8 bits
        --(*wp_register8bits);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 1u;
    }

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ld_d_n()
{
    // Variables locales
    std::uint8_t 	w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération de la valeur à charger dans le registre
    w_data8bits = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Récupération du masque du registre
    w_registerMask = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;

    // Récupération du registre
    _decodeRegister8Bits(w_registerMask, wp_register8bits, wp_register16bits);

    // Si chargement dans HL
    if (w_registerMask == 6u)
    {
        // Stockage de la valeur en mémoire à l'adresse contenue par HL
        mp_mpu->setMemVal(*wp_register16bits, w_data8bits);
    }
    else
    {
        // Mise à jour de la valeur du registre
        *wp_register8bits = w_data8bits;
    }

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_rdca()
{
    // Variables locales
    std::uint8_t w_direction = 0u;

    // Récupération du sens de rotation (0 = gauche, 1 = droite)
    w_direction = (mp_mpu->getMemVal(m_opcodeIdx) & 0x08) >> 3u;

    if (w_direction == 0u)
    {
        // Sauvegarde du MSB de A dans le flag C
        m_registers.sFlags.c = (m_registers.s8bits.a & 0x80) >> 7u;

        // Rotation circulaire de A vers la gauche
        m_registers.s8bits.a = static_cast<std::uint8_t>(((m_registers.s8bits.a) << 1u) & 0xFF) + m_registers.sFlags.c;
    }
    else
    {
        // Sauvegarde du LSB de A dans le flag C
        m_registers.sFlags.c = (m_registers.s8bits.a & 0x01);

        // Rotation ciruclaire de A vers la droite
        m_registers.s8bits.a = static_cast<std::uint8_t>(m_registers.s8bits.a >> 1u) + static_cast<std::uint8_t>(m_registers.sFlags.c << 7u);
    }

    // Gestion du flag Z
    m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ldi_hl_a()
{
    // On stocke le contenu du registre A en mémoire à l'adresse contenue par HL
    mp_mpu->setMemVal(m_registers.s16bits.hl, m_registers.s8bits.a);

    // On incrémente HL
    m_registers.s16bits.hl += 1u;

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ldi_a_hl()
{
    // On stocke le contenu de la mémoire à l'adresse contenue par HL dans A
    m_registers.s8bits.a = mp_mpu->getMemVal(m_registers.s16bits.hl);

    // On incrémente HL
    m_registers.s16bits.hl += 1u;

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ldd_hl_a()
{
    // On stocke le contenu du registre A en mémoire à l'adresse contenue par HL
    mp_mpu->setMemVal(m_registers.s16bits.hl, m_registers.s8bits.a);

    // On décrémente HL
    m_registers.s16bits.hl -= 1u;

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ldd_a_hl()
{
    // On stocke le contenu de la mémoire à l'adresse contenue par HL dans A
    m_registers.s8bits.a = mp_mpu->getMemVal(m_registers.s16bits.hl);

    // On décrémente HL
    m_registers.s16bits.hl -= 1u;

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_ld_d_d()
{
    // Variables locales
    std::uint8_t 	w_registerMask_1    = 0u;
    std::uint8_t 	w_registerMask_2    = 0u;
    std::uint8_t*	wp_register8bits_1  = NULL;
    std::uint16_t*	wp_register16bits_1 = NULL;
    std::uint8_t*	wp_register8bits_2  = NULL;
    std::uint16_t*	wp_register16bits_2 = NULL;

    // Récupération du masque du premier registre
    w_registerMask_1 = ( mp_mpu->getMemVal(m_opcodeIdx) >> 3u ) & 0x07;

    // Récupération du premier registre
    _decodeRegister8Bits(w_registerMask_1, wp_register8bits_1, wp_register16bits_1);

    // Récupération du masque du premier registre
    w_registerMask_2 = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    // Récupération du deuxième registre
    _decodeRegister8Bits(w_registerMask_2, wp_register8bits_2, wp_register16bits_2);

    // Si LD (HL),X
    if ( (w_registerMask_1 == 6u) && (w_registerMask_2 != 6u) )
    {
        // On stocke le contenu du registre wp_register8bits_2 en mémoire à l'adresse contenue par HL (wp_register16bits_1)
        mp_mpu->setMemVal(*wp_register16bits_1, *wp_register8bits_2);
    }
    // Sinon si LD X,(HL)
    else if ( (w_registerMask_1 != 6u) && (w_registerMask_2 == 6u) )
    {
        // On stocke le contenu de la mémoire à l'adresse contenue par HL (wp_register16bits_2) dans wp_register8bits_1
        *wp_register8bits_1 = mp_mpu->getMemVal(*wp_register16bits_2);
    }
    // Chargement d'un registre 8 bits dans un autre registre 8 bits
    else if ( (w_registerMask_1 != 6u) && (w_registerMask_2 != 6u) )
    {
        // On charge wp_register8bits_2 dans wp_register8bits_1
        *wp_register8bits_1 = *wp_register8bits_2;
    }

    // le cas LD (HL),(HL) revient à ne rien faire

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_jp_f_n()
{
    // Si la condition F est vraie, passage à l'instruction située à l'adresse (N) ; sinon passage à l'instruction suivante
    if (_decodeMnemonic((mp_mpu->getMemVal(m_opcodeIdx) & 0x18) >> 3u))
    {
        m_pc = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);
    }
    else
    {
        m_pc += 3u;
    }
}

void Cpu::_jp_n()
{
    // Passage à l'instruction située à l'adresse (N)
    m_pc = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);
}

void Cpu::_ld_hl_sp_plus_n()
{
    // Variables locales
    std::int8_t     w_data8bits = 0u;
    std::int32_t    w_sum       = 0;

    // Récupération de la valeur 8 bits signés
    w_data8bits = static_cast<std::int8_t>(mp_mpu->getMemVal(m_opcodeIdx + 1u));

    // Stockage dans HL du résultat de l'addition de SP par la valeur 8 bits fournie
    w_sum = static_cast<std::int32_t>(m_sp) + static_cast<std::int32_t>(w_data8bits);
    m_registers.s16bits.hl = static_cast<std::uint16_t>(w_sum & 0xFFFF);

    // Reset des flags Z et N
    m_registers.sFlags.z = 0u;
    m_registers.sFlags.n = 0u;

    // Gestion du flag C
    m_registers.sFlags.c = static_cast<std::uint8_t>((w_sum & 0x10000) == 0x10000);

    // Gestion du flag H
    m_registers.sFlags.h = static_cast<std::uint8_t>((((m_sp & 0x0FFF) + (w_data8bits & 0x0FFF)) & 0x1000) == 0x1000);

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_ld_ff00_plus_n_a()
{
    // Récupération de l'offset N
    std::uint8_t w_data8bits = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage du contenu du registre A dans la mémoire à l'adresse $FF00+N
    mp_mpu->setMemVal((0xFF00 + w_data8bits), m_registers.s8bits.a);

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_ld_a_ff00_plus_n()
{
    // Récupération de l'offset N
    std::uint8_t w_data8bits = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage dans A du contenu de la mémoire à l'adresse $FF00+N
    m_registers.s8bits.a = mp_mpu->getMemVal(0xFF00 + w_data8bits);

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_ld_c_a()
{
    // Réalise la même opération que LD (FF00+N),A
    _ld_ff00_plus_n_a();
}

void Cpu::_ld_a_c()
{
    // Réalise la même opération que LD A,(FF00+N)
    _ld_a_ff00_plus_n();
}

void Cpu::_ld_n_a()
{
    // Récupération de l'adresse d'écriture
    std::uint16_t w_data16bits = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // On stocke le contenu de A à l'adresse mémoire (N)
    mp_mpu->setMemVal(w_data16bits, m_registers.s8bits.a);

    // Mise à jour de PC
    m_pc += 3u;
}

void Cpu::_ld_a_n()
{
    // Récupération de la valeur à charger dans A
    std::uint16_t w_data16bits = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage dans A du contenu de la mémoire à l'adresse (N)
    m_registers.s8bits.a = mp_mpu->getMemVal(w_data16bits);

    // Mise à jour de PC
    m_pc += 3u;
}

void Cpu::_jp_hl()
{
    // Passage à l'instruction située à l'adresse contenue par HL
    m_pc = m_registers.s16bits.hl;
}

void Cpu::_ld_sp_hl()
{
    // Stockage du contenu de HL dans SP
    m_sp = m_registers.s16bits.hl;

    // Mise à jour de PC
    m_pc += 1u;
}
