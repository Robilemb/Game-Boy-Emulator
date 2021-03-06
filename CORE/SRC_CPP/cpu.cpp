#include <string>
#include <thread>

#include "CORE/INCLUDE/cpu.h"


// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Cpu::Cpu(Mpu* const aip_mpu) :
    m_opcodeIdx(0u),
    m_nbCycles(0u),
    mp_mpu(aip_mpu),
    m_isHalted(false)
{
    // Initialisation des masques et identifiants des opcodes
    _initOpcodesDesc();

    // Initialisation des registres
    initRegisters();

    // Initialisation de la table des données d'exécution de l'instruction DAA
    m_daaTable[0u]  = {0u, 0u, 0u, 0u, 0x0, 0x9, 0x0, 0x9, 0x00};
    m_daaTable[1u]  = {0u, 0u, 0u, 0u, 0x0, 0x8, 0xA, 0xF, 0x06};
    m_daaTable[2u]  = {0u, 1u, 0u, 0u, 0x0, 0x9, 0x0, 0x3, 0x06};
    m_daaTable[3u]  = {0u, 0u, 0u, 1u, 0xA, 0xF, 0x0, 0x9, 0x60};
    m_daaTable[4u]  = {0u, 0u, 0u, 1u, 0x9, 0xF, 0xA, 0xF, 0x66};
    m_daaTable[5u]  = {0u, 1u, 0u, 1u, 0xA, 0xF, 0x0, 0x3, 0x66};
    m_daaTable[6u]  = {0u, 0u, 1u, 1u, 0x0, 0x2, 0x0, 0x9, 0x60};
    m_daaTable[7u]  = {0u, 0u, 1u, 1u, 0x0, 0x2, 0xA, 0xF, 0x66};
    m_daaTable[8u]  = {0u, 1u, 1u, 1u, 0x0, 0x3, 0x0, 0x3, 0x66};
    m_daaTable[9u]  = {1u, 0u, 0u, 0u, 0x0, 0x9, 0x0, 0x9, 0x00};
    m_daaTable[10u] = {1u, 1u, 0u, 0u, 0x0, 0x8, 0x6, 0xF, 0xFA};
    m_daaTable[11u] = {1u, 0u, 1u, 1u, 0x7, 0xF, 0x0, 0x9, 0xA0};
    m_daaTable[12u] = {1u, 1u, 1u, 1u, 0x6, 0xF, 0x6, 0xF, 0x9A};    
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

// Accesseur sur le nombre de cycles de l'instruction courante
std::uint8_t Cpu::getNbCyccles() const
{
    return m_nbCycles;
}

// Accesseur sur le flag d'activation des interruptions
std::uint8_t Cpu::getRegisterIME() const
{
    return m_ime;
}

bool Cpu::getIsHalted() const
{
    return m_isHalted;
}


// ********************************************************
// INITIALISATION DES REGISTRES
// ********************************************************

void Cpu::initRegisters()
{
    // Initialisation des registres
    m_registers.s8bits.a    = 0x00;
    m_registers.s8bits.f    = 0x00;
    m_registers.s16bits.bc  = 0x0000;
    m_registers.s16bits.de  = 0x0000;
    m_registers.s16bits.hl  = 0x0000;
    m_sp                    = 0x0000;
    m_pc                    = 0x0000;
    m_ime                   = 0x00;
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
    m_opcodesDesc.masque8bits[24] = 0xC0; m_opcodesDesc.id8bits[24] = 0x40; m_opcodesDesc.execute8bits[24]  = &Cpu::_ld_d_d;              // LD D,D / HALT
    m_opcodesDesc.masque8bits[25] = 0xC0; m_opcodesDesc.id8bits[25] = 0x80; m_opcodesDesc.execute8bits[25]  = &Cpu::_alu_a_d;             // ALU A,D
    m_opcodesDesc.masque8bits[26] = 0xC7; m_opcodesDesc.id8bits[26] = 0xC6; m_opcodesDesc.execute8bits[26]  = &Cpu::_alu_a_n;             // ALU A,N
    m_opcodesDesc.masque8bits[27] = 0xCF; m_opcodesDesc.id8bits[27] = 0xC1; m_opcodesDesc.execute8bits[27]  = &Cpu::_pop_r;               // POP R
    m_opcodesDesc.masque8bits[28] = 0xCF; m_opcodesDesc.id8bits[28] = 0xC5; m_opcodesDesc.execute8bits[28]  = &Cpu::_push_r;              // PUSH R
    m_opcodesDesc.masque8bits[29] = 0xC7; m_opcodesDesc.id8bits[29] = 0xC7; m_opcodesDesc.execute8bits[29]  = &Cpu::_rst_n;               // RST N
    m_opcodesDesc.masque8bits[30] = 0xE7; m_opcodesDesc.id8bits[30] = 0xC0; m_opcodesDesc.execute8bits[30]  = &Cpu::_ret_f;               // RET F
    m_opcodesDesc.masque8bits[31] = 0xFF; m_opcodesDesc.id8bits[31] = 0xC9; m_opcodesDesc.execute8bits[31]  = &Cpu::_ret;                 // RET
    m_opcodesDesc.masque8bits[32] = 0xFF; m_opcodesDesc.id8bits[32] = 0xD9; m_opcodesDesc.execute8bits[32]  = &Cpu::_reti;                // RETI
    m_opcodesDesc.masque8bits[33] = 0xE7; m_opcodesDesc.id8bits[33] = 0xC2; m_opcodesDesc.execute8bits[33]  = &Cpu::_jp_f_n;              // JP F,N
    m_opcodesDesc.masque8bits[34] = 0xFF; m_opcodesDesc.id8bits[34] = 0xC3; m_opcodesDesc.execute8bits[34]  = &Cpu::_jp_n;                // JP N
    m_opcodesDesc.masque8bits[35] = 0xE7; m_opcodesDesc.id8bits[35] = 0xC4; m_opcodesDesc.execute8bits[35]  = &Cpu::_call_f_n;            // CALL F,N
    m_opcodesDesc.masque8bits[36] = 0xFF; m_opcodesDesc.id8bits[36] = 0xCD; m_opcodesDesc.execute8bits[36]  = &Cpu::_call_n;              // CALL N
    m_opcodesDesc.masque8bits[37] = 0xFF; m_opcodesDesc.id8bits[37] = 0xE8; m_opcodesDesc.execute8bits[37]  = &Cpu::_add_sp_n;            // ADD SP,N
    m_opcodesDesc.masque8bits[38] = 0xFF; m_opcodesDesc.id8bits[38] = 0xF8; m_opcodesDesc.execute8bits[38]  = &Cpu::_ld_hl_sp_plus_n;     // LD HL,SP+N
    m_opcodesDesc.masque8bits[39] = 0xFF; m_opcodesDesc.id8bits[39] = 0xE0; m_opcodesDesc.execute8bits[39]  = &Cpu::_ld_ff00_plus_n_a;    // LD (FF00+N),A
    m_opcodesDesc.masque8bits[40] = 0xFF; m_opcodesDesc.id8bits[40] = 0xF0; m_opcodesDesc.execute8bits[40]  = &Cpu::_ld_a_ff00_plus_n;    // LD A,(FF00+N)
    m_opcodesDesc.masque8bits[41] = 0xFF; m_opcodesDesc.id8bits[41] = 0xE2; m_opcodesDesc.execute8bits[41]  = &Cpu::_ld_c_a;              // LD (C),A
    m_opcodesDesc.masque8bits[42] = 0xFF; m_opcodesDesc.id8bits[42] = 0xF2; m_opcodesDesc.execute8bits[42]  = &Cpu::_ld_a_c;              // LD A,(C)
    m_opcodesDesc.masque8bits[43] = 0xFF; m_opcodesDesc.id8bits[43] = 0xEA; m_opcodesDesc.execute8bits[43]  = &Cpu::_ld_n_a;              // LD (N),A
    m_opcodesDesc.masque8bits[44] = 0xFF; m_opcodesDesc.id8bits[44] = 0xFA; m_opcodesDesc.execute8bits[44]  = &Cpu::_ld_a_n;              // LD A,(N)
    m_opcodesDesc.masque8bits[45] = 0xFF; m_opcodesDesc.id8bits[45] = 0xE9; m_opcodesDesc.execute8bits[45]  = &Cpu::_jp_hl;               // JP HL
    m_opcodesDesc.masque8bits[46] = 0xFF; m_opcodesDesc.id8bits[46] = 0xF9; m_opcodesDesc.execute8bits[46]  = &Cpu::_ld_sp_hl;            // LD SP,HL
    m_opcodesDesc.masque8bits[47] = 0xFF; m_opcodesDesc.id8bits[47] = 0xF3; m_opcodesDesc.execute8bits[47]  = &Cpu::_di;                  // DI
    m_opcodesDesc.masque8bits[48] = 0xFF; m_opcodesDesc.id8bits[48] = 0xFB; m_opcodesDesc.execute8bits[48]  = &Cpu::_ei;                  // EI
    m_opcodesDesc.masque8bits[49] = 0xFF; m_opcodesDesc.id8bits[49] = 0xCB; m_opcodesDesc.execute8bits[49]  = &Cpu::_execute16bOpcode;    // 16b opcode

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
    // Variables locales
    std::uint16_t                                               w_i       = 0u;
    std::uint16_t                                               w_id      = 0u;
    std::uint8_t                                                w_opcode  = mp_mpu->getMemVal(ai_opcodeIdx);

    if (m_isHalted == false)
    {
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

        // Le flag res ne peut être que reset
        if (m_registers.sFlags.res != 0u)
            m_registers.sFlags.res = 0u;
    }
}

// ********************************************************
// EXECUTION D'UNE INTERRUPTION
// ********************************************************

void Cpu::executeInterrupt(const te_interupts ai_interrupt)
{
    // Valeur du registre IF
    std::uint8_t w_ifRegister = mp_mpu->getMemVal(MPU_IF_ADDRESS);

    // Désactivation des interruptions
    m_ime = 0u;

    // Sauvegarde du MSW de PC à l'adresse mémoire de SP - 1
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 1u), static_cast<std::uint8_t>((m_pc & 0xFF00) >> 8u));

    // Sauvegarde du LSW de PC à l'adresse mémoire de SP - 2
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 2u), static_cast<std::uint8_t>(m_pc & 0x00FF));

    // SP = SP - 2
    m_sp -= 2u;

    // Sortie de pause si besoin
    m_isHalted = false;

    // Exécution de l'interruption demandée
    switch (ai_interrupt)
    {
        case E_VBLANK :
            // Requete prise en compte
            mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister & ~GAMEBOY_VBLANK_REQUESTED));

            // Mise à jour de PC à l'addresse du code l'interuption
            m_pc = CPU_VBLANK_ADDRESS;

            break;

        case E_LCD_STAT :
            // Requete prise en compte
            mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister & ~GAMEBOY_LCD_STAT_REQUESTED));

            // Mise à jour de PC à l'addresse du code l'interuption
            m_pc = CPU_LCD_STAT_ADDRESS;

            break;

        case E_TIMER :
            // Requete prise en compte
            mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister & ~GAMEBOY_TIMER_REQUESTED));

            // Mise à jour de PC à l'addresse du code l'interuption
            m_pc = CPU_TIMER_ADDRESS;

            break;

        case E_SERIAL :
            // Requete prise en compte
            mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister & ~GAMEBOY_SERIAL_REQUESTED));

            // Mise à jour de PC à l'addresse du code l'interuption
            m_pc = CPU_SERIAL_ADDRESS;

            break;

        case E_JOYPAD :
            // Requete prise en compte
            mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister & ~GAMEBOY_JOYPAD_REQUESTED));

            // Mise à jour de PC à l'addresse du code l'interuption
            m_pc = CPU_JOYPAD_ADDRESS;

            break;

        default :
            std::cout << "Erreur : interruption inconnue" << std::endl;
            exit(-1);
    }

    // Mise à jour du nombre de cycles
    m_nbCycles = 6u;
}


// *****************
// Fonctions privées
// *****************

bool Cpu::_decodeMnemonic(const std::uint8_t ai_mnemo)
{
    // Variables locales
    bool w_test = true;

    // Recherche du mnémonique
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
    }

    return w_test;
}

void Cpu::_decodeRegister8Bits(const std::uint8_t ai_registerMask, std::uint8_t* &aop_register8bits)
{
    // Recherche du registre 8 bits
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

        case 7u:
            aop_register8bits = &m_registers.s8bits.a;
            break;

        default:
            std::cout << "ERREUR : Registre 8 bits inconnu" << std::endl;
            exit(-1);
    }
}

void Cpu::_decodeRegister16Bits(const std::uint8_t ai_registerMask, std::uint16_t* &aop_register16bits)
{
    // Variables locales
    std::uint8_t w_opcodeId = mp_mpu->getMemVal(m_opcodeIdx) & 0xCF;

    // Recherche du registre 16 bits
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
            if ((w_opcodeId == 0xC1) || (w_opcodeId == 0xC5))
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
    }
}

void Cpu::_decodeAndRunALU(const std::uint8_t ai_aluMask, const std::uint8_t ai_operand)
{
    // Variables locales
    std::uint16_t w_sum     = 0u;
    std::uint16_t w_diff    = 0u;

    // Recherche et exécution de l'opération à effectuer
    switch (ai_aluMask)
    {
        // ADD
        case 0u:
            // Calcul de la somme entre A et l'opérande
            w_sum = static_cast<std::uint16_t>(m_registers.s8bits.a) + static_cast<std::uint16_t>(ai_operand);

            // Gestion du flags H
            m_registers.sFlags.h = static_cast<std::uint8_t>((((m_registers.s8bits.a & 0x0F) + (ai_operand & 0x0F)) & 0x10) == 0x10);

            // Sauvegarde de la somme dans A
            m_registers.s8bits.a = static_cast<std::uint8_t>(w_sum & 0xFF);

            // Gestion des flags N, Z et C
            m_registers.sFlags.n = 0u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.c = static_cast<std::uint8_t>((w_sum & 0x100) == 0x100);

            break;

        // ADC
        case 1u:
            // Calcul de la somme entre A, l'opérande et le flag C
            w_sum = static_cast<std::uint16_t>(m_registers.s8bits.a) + static_cast<std::uint16_t>(ai_operand) + static_cast<std::uint8_t>(m_registers.sFlags.c);

            // Gestion du flags H
            m_registers.sFlags.h = static_cast<std::uint8_t>((((m_registers.s8bits.a & 0x0F) + (ai_operand & 0x0F) + m_registers.sFlags.c) & 0x10) == 0x10);

            // Sauvegarde de la somme dans A
            m_registers.s8bits.a = static_cast<std::uint8_t>(w_sum & 0xFF);

            // Gestion des flags N, Z et C
            m_registers.sFlags.n = 0u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.c = static_cast<std::uint8_t>((w_sum & 0x100) == 0x100);

            break;

        // SUB
        case 2u:
            // Calcul de la soustraction entre A et l'opérande
            w_diff = static_cast<std::uint16_t>(m_registers.s8bits.a) - static_cast<std::uint16_t>(ai_operand);

            // Gestion du flags H
            m_registers.sFlags.h = static_cast<std::uint8_t>((((m_registers.s8bits.a & 0x0F) - (ai_operand & 0x0F)) & 0x10) == 0x10);

            // Sauvegarde de la différence dans A
            m_registers.s8bits.a = static_cast<std::uint8_t>(w_diff & 0xFF);

            // Gestion des flags N, Z et C
            m_registers.sFlags.n = 1u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.c = static_cast<std::uint8_t>((w_diff & 0x100) == 0x100);

            break;

        // SBC
        case 3u:
            // Calcul de la soustraction entre A et l'opérande
            w_diff = static_cast<std::uint16_t>(m_registers.s8bits.a) - static_cast<std::uint16_t>(ai_operand) - static_cast<std::uint8_t>(m_registers.sFlags.c);

            // Gestion du flags H
            m_registers.sFlags.h = static_cast<std::uint8_t>((((m_registers.s8bits.a & 0x0F) - (ai_operand & 0x0F) - m_registers.sFlags.c) & 0x10) == 0x10);

            // Sauvegarde de la différence dans A
            m_registers.s8bits.a = static_cast<std::uint8_t>(w_diff & 0xFF);

            // Gestion des flags N, Z et C
            m_registers.sFlags.n = 1u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.c = static_cast<std::uint8_t>((w_diff & 0x100) == 0x100);

            break;

        // AND
        case 4u:
            // ET logique entre A et l'opérande
            m_registers.s8bits.a &= ai_operand;

            // Gestion des flags
            m_registers.sFlags.n = 0u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.h = 1u;
            m_registers.sFlags.c = 0u;

            break;

        // XOR
        case 5u:
            // OU EXCLUSIF logique entre A et l'opérande
            m_registers.s8bits.a ^= ai_operand;

            // Gestion des flags
            m_registers.sFlags.n = 0u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.h = 0u;
            m_registers.sFlags.c = 0u;

            break;

        // OR
        case 6u:
            // OU logique entre A et l'opérande
            m_registers.s8bits.a |= ai_operand;

            // Gestion des flags
            m_registers.sFlags.n = 0u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);
            m_registers.sFlags.h = 0u;
            m_registers.sFlags.c = 0u;

            break;

        // CP
        case 7u:
            // Calcul de la soustraction entre A et l'opérande
            w_diff = static_cast<std::uint16_t>(m_registers.s8bits.a) - static_cast<std::uint16_t>(ai_operand);

            // Gestion du flags H
            m_registers.sFlags.h = static_cast<std::uint8_t>((((m_registers.s8bits.a & 0x0F) - (ai_operand & 0x0F)) & 0x10) == 0x10);

            // Gestion des flags N, Z et C
            m_registers.sFlags.n = 1u;
            m_registers.sFlags.z = static_cast<std::uint8_t>(w_diff == 0u);
            m_registers.sFlags.c = static_cast<std::uint8_t>((w_diff & 0x100) == 0x100);

            break;

        default:
            std::cout << "Erreur : Opérateur ALU inconnu" << std::endl;
            exit(-1);
    }
}


// **************************
// Implémentation des opcodes
// **************************

void Cpu::_nop()
{
    // Passage à l'instruction suivante
    m_pc = m_pc + 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 5u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 3u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
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

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_inc_d()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint8_t    w_valueAtHL         = 0u;

    // Récupération du masque du registre 8b
    w_registerMask = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u);

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur à l'adresse contenue par HL
        w_valueAtHL = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>((((w_valueAtHL & 0x0F) + 1u) & 0x10) == 0x10);

        // Incrémentation de la valeur à l'adresse contenue par HL
        mp_mpu->setMemVal(m_registers.s16bits.hl, ++w_valueAtHL);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(w_valueAtHL == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 0u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 3u;
    }
    else
    {
        // Récupération du registre 8b
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>((((*wp_register8bits & 0x0F) + 1u) & 0x10) == 0x10);

        // Décrémentation du registre 8 bits
        ++(*wp_register8bits);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 0u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 1u;
    }

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_dec_d()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;
    std::uint8_t    w_valueAtHL         = 0u;

    // Récupération du masque du registre 8b
    w_registerMask = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u);

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur à l'adresse contenue par HL
        w_valueAtHL = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>((((w_valueAtHL & 0x0F) - 1u) & 0x10) == 0x10);

        // Décrémentation de la valeur à l'adresse contenue par HL
        mp_mpu->setMemVal(m_registers.s16bits.hl, --w_valueAtHL);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(w_valueAtHL == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 1u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 3u;
    }
    else
    {
        // Récupération du registre 8b
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Gestion du flag H
        m_registers.sFlags.h = static_cast<std::uint8_t>((((*wp_register8bits & 0x0F) - 1u) & 0x10) == 0x10);

        // Décrémentation du registre 8 bits
        --(*wp_register8bits);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Gestion du flag N
        m_registers.sFlags.n = 1u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 1u;
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

    // Récupération de la valeur à charger dans le registre
    w_data8bits = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Récupération du masque du registre
    w_registerMask = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;

    // Si chargement dans HL
    if (w_registerMask == 6u)
    {
        // Stockage de la valeur en mémoire à l'adresse contenue par HL
        mp_mpu->setMemVal(m_registers.s16bits.hl, w_data8bits);

        // Mise à jour du nombre de cycles
        m_nbCycles = 3u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Mise à jour de la valeur du registre
        *wp_register8bits = w_data8bits;

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
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

    // Reset des flags Z, H et N
    m_registers.sFlags.z = 0u;
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_rda()
{
    // Variables locales
    std::uint8_t w_direction = 0u;
    std::uint8_t w_flagCSave = 0u;

    // Récupération du sens de rotation (0 = gauche, 1 = droite)
    w_direction = (mp_mpu->getMemVal(m_opcodeIdx) & 0x08) >> 3u;

    // Sauvegarde du flag C
    w_flagCSave = m_registers.sFlags.c;

    if (w_direction == 0u)
    {
        // Sauvegarde du MSB de A dans le flag C
        m_registers.sFlags.c = (m_registers.s8bits.a & 0x80) >> 7u;

        // Rotation de A vers la gauche et recopie de la sauvegarde du flag C sur le LSB de A
        m_registers.s8bits.a = static_cast<std::uint8_t>((m_registers.s8bits.a << 1u) & 0xFF) + w_flagCSave;
    }
    else
    {
        // Sauvegarde du LSB de A dans le flag C
        m_registers.sFlags.c = (m_registers.s8bits.a & 0x01);

        // Rotation de A vers la droite et recopie de la sauvegarde du flag C sur le MSB de A
        m_registers.s8bits.a = static_cast<std::uint8_t>(m_registers.s8bits.a >> 1u) + static_cast<std::uint8_t>(w_flagCSave << 7u);
    }

    // Reset des flags Z, H et N
    m_registers.sFlags.z = 0u;
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_stop()
{
    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_jr_n()
{
    // Passage à l'instruction située à l'adresse PC + N
    m_pc += static_cast<std::int8_t>(mp_mpu->getMemVal(m_opcodeIdx + 1u)) + 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 3u;
}

void Cpu::_jr_f_n()
{
    // Si la condition F est vraie
    if (_decodeMnemonic((mp_mpu->getMemVal(m_opcodeIdx) & 0x18) >> 3u))
    {
        // Passage à l'instruction située à l'adresse PC + N
        _jr_n();
    }
    else
    {
        // Passage à l'instruction suivante
        m_pc += 2u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }
}

void Cpu::_ldi_hl_a()
{
    // On stocke le contenu du registre A en mémoire à l'adresse contenue par HL
    mp_mpu->setMemVal(m_registers.s16bits.hl, m_registers.s8bits.a);

    // On incrémente HL
    m_registers.s16bits.hl += 1u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_ldi_a_hl()
{
    // On stocke le contenu de la mémoire à l'adresse contenue par HL dans A
    m_registers.s8bits.a = mp_mpu->getMemVal(m_registers.s16bits.hl);

    // On incrémente HL
    m_registers.s16bits.hl += 1u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_ldd_hl_a()
{
    // On stocke le contenu du registre A en mémoire à l'adresse contenue par HL
    mp_mpu->setMemVal(m_registers.s16bits.hl, m_registers.s8bits.a);

    // On décrémente HL
    m_registers.s16bits.hl -= 1u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_ldd_a_hl()
{
    // On stocke le contenu de la mémoire à l'adresse contenue par HL dans A
    m_registers.s8bits.a = mp_mpu->getMemVal(m_registers.s16bits.hl);

    // On décrémente HL
    m_registers.s16bits.hl -= 1u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_cpl()
{
    // Complément à 1 de A
    m_registers.s8bits.a = ~m_registers.s8bits.a;

    // Gestion des flags N et H
    m_registers.sFlags.n = 1u;
    m_registers.sFlags.h = 1u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_daa()
{
    // Variables locales
    std::uint8_t w_idx  = 0u;
    std::uint8_t w_AMsw = (m_registers.s8bits.a & 0xF0) >> 4u;
    std::uint8_t w_ALsw = m_registers.s8bits.a & 0x0F;

    std::uint8_t w_possibilities[CPU_DAA_TABLE_NB] = {0u};
    std::uint8_t w_max      = 0u;
    std::uint8_t w_maxIdx   = 0u;

    // Recherche de la configuration à exécuter
    for (w_idx = 0u; w_idx < CPU_DAA_TABLE_NB; ++w_idx)
    {
        if (    (m_registers.sFlags.n == m_daaTable[w_idx].flagN)
            &&  (m_registers.sFlags.h == m_daaTable[w_idx].flagH)
            &&  (m_registers.sFlags.c == m_daaTable[w_idx].flagCCur))
        {
            if (w_AMsw >= m_daaTable[w_idx].minAMsw) w_possibilities[w_idx]++;
            if (w_AMsw <= m_daaTable[w_idx].maxAMsw) w_possibilities[w_idx]++;
            if (w_ALsw >= m_daaTable[w_idx].minALsw) w_possibilities[w_idx]++;
            if (w_ALsw <= m_daaTable[w_idx].maxALsw) w_possibilities[w_idx]++;
        }
    }

    for (w_idx = 0u; w_idx < CPU_DAA_TABLE_NB; ++w_idx)
    {
        if (w_possibilities[w_idx] > w_max)
        {
            w_max       = w_possibilities[w_idx];
            w_maxIdx    = w_idx;
        }
    }

    // Ajoute à A la valeur correspondante à la configuration courante
    m_registers.s8bits.a = static_cast<std::uint8_t>((static_cast<std::uint16_t>(m_registers.s8bits.a) + static_cast<std::uint16_t>(m_daaTable[w_maxIdx].addValue)) & 0xFF);

    // Gestion du flag C
    m_registers.sFlags.c = m_daaTable[w_maxIdx].flagCNew;

    // Gestion du flag Z
    m_registers.sFlags.z = static_cast<std::uint8_t>(m_registers.s8bits.a == 0u);

    // Reset du flag H
    m_registers.sFlags.h = 0u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_scf()
{
    // C = 1
    m_registers.sFlags.c = 1u;

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_ccf()
{
    // Flip C
    if (m_registers.sFlags.c == 1u)
    {
        m_registers.sFlags.c = 0u;
    }
    else
    {
        m_registers.sFlags.c = 1u;
    }

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_ld_d_d()
{
    // Variables locales
    std::uint8_t 	w_registerMask_1    = 0u;
    std::uint8_t 	w_registerMask_2    = 0u;
    std::uint8_t*	wp_register8bits_1  = NULL;
    std::uint8_t*	wp_register8bits_2  = NULL;

    // Récupération du masque du premier registre
    w_registerMask_1 = ( mp_mpu->getMemVal(m_opcodeIdx) >> 3u ) & 0x07;

    // Récupération du masque du premier registre
    w_registerMask_2 = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    // Si LD (HL),X
    if ( (w_registerMask_1 == 6u) && (w_registerMask_2 != 6u) )
    {
        // Récupération du deuxième registre
        _decodeRegister8Bits(w_registerMask_2, wp_register8bits_2);

        // On stocke le contenu du registre wp_register8bits_2 en mémoire à l'adresse contenue par HL
        mp_mpu->setMemVal(m_registers.s16bits.hl, *wp_register8bits_2);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }
    // Sinon si LD X,(HL)
    else if ( (w_registerMask_1 != 6u) && (w_registerMask_2 == 6u) )
    {
        // Récupération du premier registre
        _decodeRegister8Bits(w_registerMask_1, wp_register8bits_1);

        // On stocke le contenu de la mémoire à l'adresse contenue par HL dans wp_register8bits_1
        *wp_register8bits_1 = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }
    // Chargement d'un registre 8 bits dans un autre registre 8 bits
    else if ( (w_registerMask_1 != 6u) && (w_registerMask_2 != 6u) )
    {
        // Récupération du premier registre
        _decodeRegister8Bits(w_registerMask_1, wp_register8bits_1);

        // Récupération du deuxième registre
        _decodeRegister8Bits(w_registerMask_2, wp_register8bits_2);

        // On charge wp_register8bits_2 dans wp_register8bits_1
        *wp_register8bits_1 = *wp_register8bits_2;

        // Mise à jour du nombre de cycles
        m_nbCycles = 1u;
    }
    else    // le cas LD (HL),(HL) correspond à l'instruction HALT
    {
        if (m_ime == 1u)
        {
            // Pause du CPU
            m_isHalted = true;
        }
        else
        {
            // L'instruction suivante ne sera pas exécutée
            m_pc += 1u;
        }

        // Mise à jour du nombre de cycles
        m_nbCycles = 1u;
    }

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_alu_a_d()
{
    // Variables locales
    std::uint8_t    w_aluMask           = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    // Si registre HL
    if (w_registerMask == 6u)
    {
        // Lecture de la valeur à l'adresse (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Lecture de la valeur du registre 8b
        w_data8bits = *wp_register8bits;

        // Mise à jour du nombre de cycles
        m_nbCycles = 1u;
    }

    // Opération ALU
    _decodeAndRunALU(w_aluMask, w_data8bits);

    // Mise à jour de PC
    m_pc += 1u;
}

void Cpu::_alu_a_n()
{
    // Variables locales
    std::uint8_t w_aluMask      = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;
    std::uint8_t w_data8bits    = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Opération ALU
    _decodeAndRunALU(w_aluMask, w_data8bits);

    // Mise à jour de PC
    m_pc += 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_pop_r()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération du registre 16b
    w_registerMask = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x30) >> 4u);
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Sauvegarde dans le registre 16b des données contenues aux adresses SP (LSW) et SP + 1 (MSW)
    *wp_register16bits = (static_cast<std::uint16_t>(mp_mpu->getMemVal(m_sp + 1u)) << 8u) + static_cast<std::uint16_t>(mp_mpu->getMemVal(m_sp));

    // SP = SP + 2
    m_sp += 2u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 3u;
}

void Cpu::_push_r()
{
    // Variables locales
    std::uint8_t 	w_registerMask      = 0u;
    std::uint16_t*	wp_register16bits   = NULL;

    // Récupération du registre 16b
    w_registerMask = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x30) >> 4u);
    _decodeRegister16Bits(w_registerMask, wp_register16bits);

    // Sauvegarde du MSW du registre 16b à l'adresse mémoire de SP - 1
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 1u), static_cast<std::uint8_t>((*wp_register16bits & 0xFF00) >> 8u));

    // Sauvegarde du LSW du registre 16b à l'adresse mémoire de SP - 2
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 2u), static_cast<std::uint8_t>(*wp_register16bits & 0x00FF));

    // SP = SP - 2
    m_sp -= 2u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_rst_n()
{
    // Récupération de la commande de reset
    std::uint8_t w_rstCmd = ((mp_mpu->getMemVal(m_opcodeIdx) & 0x38)) >> 3u;

    // Adresse de l'instruction suivante
    std::uint16_t w_nextInstruction = m_pc + 1u;

    // Sauvegarde du MSW de PC à l'adresse mémoire de SP - 1
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 1u), static_cast<std::uint8_t>((w_nextInstruction & 0xFF00) >> 8u));

    // Sauvegarde du LSW de PC à l'adresse mémoire de SP - 2
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 2u), static_cast<std::uint8_t>(w_nextInstruction & 0x00FF));

    // SP = SP - 2
    m_sp -= 2u;

    // Reset de PC en fonction de N
    m_pc = static_cast<std::uint16_t>(w_rstCmd) << 3u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_ret_f()
{
    // Si la condition F est vraie, sauvegarde dans PC des données contenues aux adresses SP (LSW) et SP + 1 (MSW) ; sinon passage à l'instruction suivante
    if (_decodeMnemonic((mp_mpu->getMemVal(m_opcodeIdx) & 0x18) >> 3u))
    {
        // Retour de fonction
        _ret();

        // Mise à jour du nombre de cycles
        m_nbCycles = 5u;
    }
    else
    {
        // Mise à jour de PC
        m_pc += 1u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }
}

void Cpu::_ret()
{
    // Sauvegarde dans PC des données contenues aux adresses SP (LSW) et SP + 1 (MSW)
    m_pc = (static_cast<std::uint16_t>(mp_mpu->getMemVal(m_sp + 1u)) << 8u) + static_cast<std::uint16_t>(mp_mpu->getMemVal(m_sp));

    // SP = SP + 2
    m_sp += 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_reti()
{
    // Interruptions réactivées
    m_ime = 1u;

    // Sortie de l'interruption
    _ret();
}

void Cpu::_jp_f_n()
{
    // Si la condition F est vraie, passage à l'instruction située à l'adresse (N) ; sinon passage à l'instruction suivante
    if (_decodeMnemonic((mp_mpu->getMemVal(m_opcodeIdx) & 0x18) >> 3u))
    {
        _jp_n();
    }
    else
    {
        // Mise à jour de PC
        m_pc += 3u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 3u;
    }
}

void Cpu::_jp_n()
{
    // Passage à l'instruction située à l'adresse (N)
    m_pc = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_call_f_n()
{
    // Si la condition F est vraie
    if (_decodeMnemonic((mp_mpu->getMemVal(m_opcodeIdx) & 0x18) >> 3u))
    {
        // Passage à l'instruction située à l'adresse (N)
        _call_n();
    }
    else
    {
        // Passage à l'instruction suivante
        m_pc += 3u;

        // Mise à jour du nombre de cycles
        m_nbCycles = 3u;
    }
}

void Cpu::_call_n()
{
    // Adresse de l'instruction suivante
    std::uint16_t w_nextInstruction = m_pc + 3u;

    // Sauvegarde du MSW de PC à l'adresse mémoire de SP - 1
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 1u), static_cast<std::uint8_t>((w_nextInstruction & 0xFF00) >> 8u));

    // Sauvegarde du LSW de PC à l'adresse mémoire de SP - 2
    mp_mpu->setMemVal(static_cast<std::uint16_t>(m_sp - 2u), static_cast<std::uint8_t>(w_nextInstruction & 0x00FF));

    // SP = SP - 2
    m_sp -= 2u;

    // Passage à l'instruction située à l'adresse (N)
    _jp_n();

    // Mise à jour du nombre de cycles
    m_nbCycles = 6u;
}

void Cpu::_add_sp_n()
{
    // Variables locales
    std::int8_t     w_data8bits = 0;
    std::int32_t    w_sum       = 0;

    // Récupération de la valeur 8 bits signés
    w_data8bits = static_cast<std::int8_t>(mp_mpu->getMemVal(m_opcodeIdx + 1u));

    // Gestion du flag H
    m_registers.sFlags.h = static_cast<std::uint8_t>((((m_sp & 0x0F) + (w_data8bits & 0x0F)) & 0x10) == 0x10);

    // Gestion du flag C
    m_registers.sFlags.c = static_cast<std::uint8_t>((((m_sp & 0xFF) + (w_data8bits & 0xFF)) & 0x100) == 0x100);

    // Stockage dans SP du résultat de l'addition de SP par la valeur 8 bits fournie
    w_sum = static_cast<std::int32_t>(m_sp) + static_cast<std::int32_t>(w_data8bits);
    m_sp = static_cast<std::uint16_t>(w_sum & 0xFFFF);

    // Reset des flags Z et N
    m_registers.sFlags.z = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_ld_hl_sp_plus_n()
{
    // Variables locales
    std::int8_t     w_data8bits = 0;
    std::int32_t    w_sum       = 0;

    // Récupération de la valeur 8 bits signés
    w_data8bits = static_cast<std::int8_t>(mp_mpu->getMemVal(m_opcodeIdx + 1u));

    // Gestion du flag H
    m_registers.sFlags.h = static_cast<std::uint8_t>((((m_sp & 0x0F) + (w_data8bits & 0x0F)) & 0x10) == 0x10);

    // Gestion du flag C
    m_registers.sFlags.c = static_cast<std::uint8_t>((((m_sp & 0xFF) + (w_data8bits & 0xFF)) & 0x100) == 0x100);

    // Stockage dans HL du résultat de l'addition de SP par la valeur 8 bits fournie
    w_sum = static_cast<std::int32_t>(m_sp) + static_cast<std::int32_t>(w_data8bits);
    m_registers.s16bits.hl = static_cast<std::uint16_t>(w_sum & 0xFFFF);

    // Reset des flags Z et N
    m_registers.sFlags.z = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 3u;
}

void Cpu::_ld_ff00_plus_n_a()
{
    // Récupération de l'offset N
    std::uint8_t w_data8bits = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage du contenu du registre A dans la mémoire à l'adresse $FF00+N
    mp_mpu->setMemVal((0xFF00 + w_data8bits), m_registers.s8bits.a);

    // Mise à jour de PC
    m_pc += 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 3u;
}

void Cpu::_ld_a_ff00_plus_n()
{
    // Récupération de l'offset N
    std::uint8_t w_data8bits = mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage dans A du contenu de la mémoire à l'adresse $FF00+N
    m_registers.s8bits.a = mp_mpu->getMemVal(0xFF00 + w_data8bits);

    // Mise à jour de PC
    m_pc += 2u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 3u;
}

void Cpu::_ld_c_a()
{
    // Stockage du contenu du registre A dans la mémoire à l'adresse $FF00+C
    mp_mpu->setMemVal((0xFF00 + m_registers.s8bits.c), m_registers.s8bits.a);

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_ld_a_c()
{
    // Stockage dans A du contenu de la mémoire à l'adresse $FF00+C
    m_registers.s8bits.a = mp_mpu->getMemVal(0xFF00 + m_registers.s8bits.c);

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_ld_n_a()
{
    // Récupération de l'adresse d'écriture
    std::uint16_t w_data16bits = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // On stocke le contenu de A à l'adresse mémoire (N)
    mp_mpu->setMemVal(w_data16bits, m_registers.s8bits.a);

    // Mise à jour de PC
    m_pc += 3u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_ld_a_n()
{
    // Récupération de la valeur à charger dans A
    std::uint16_t w_data16bits = (mp_mpu->getMemVal(m_opcodeIdx + 2u) << 8u) + mp_mpu->getMemVal(m_opcodeIdx + 1u);

    // Stockage dans A du contenu de la mémoire à l'adresse (N)
    m_registers.s8bits.a = mp_mpu->getMemVal(w_data16bits);

    // Mise à jour de PC
    m_pc += 3u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 4u;
}

void Cpu::_jp_hl()
{
    // Passage à l'instruction située à l'adresse contenue par HL
    m_pc = m_registers.s16bits.hl;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_ld_sp_hl()
{
    // Stockage du contenu de HL dans SP
    m_sp = m_registers.s16bits.hl;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 2u;
}

void Cpu::_di()
{
    // Interruptions désactivées
    m_ime = 0u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_ei()
{
    // Interruptions activées
    m_ime = 1u;

    // Mise à jour de PC
    m_pc += 1u;

    // Mise à jour du nombre de cycles
    m_nbCycles = 1u;
}

void Cpu::_execute16bOpcode()
{
    // Variables locales
    std::uint16_t   w_i       = 0u;
    std::uint16_t   w_id      = 0u;
    std::uint8_t    w_opcode  = mp_mpu->getMemVal(++m_opcodeIdx);

    // Récupération de l'ID de l'opcode à executer
    for (w_i = 0u; w_i < CPU_NB_OPCODES_16_BITS; ++w_i)
    {
        // Recherche de l'ID correspondant à l'opcode 16 bits
        w_id = (m_opcodesDesc.masque16bitsLSW[w_i] & w_opcode);

        if (w_id == m_opcodesDesc.id16bitsLSW[w_i])
        {
            // Exécution de l'opcode
            (this->*m_opcodesDesc.execute16bits[w_i])();
            break;
        }
    }

    // Gestion opcode inconnu
    if (w_i == CPU_NB_OPCODES_16_BITS)
    {
        std::cout << "Erreur : opcode inconnu" << std::endl;
        exit(-1);
    }
}

void Cpu::_rdc_d()
{
    // Variables locales
    std::uint8_t    w_direction         = 0u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du sens de rotation (0 = gauche, 1 = droite)
    w_direction = (mp_mpu->getMemVal(m_opcodeIdx) & 0x08) >> 3u;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        if (w_direction == 0u)
        {
            // Sauvegarde du MSB de (HL) dans le flag C
            m_registers.sFlags.c = (w_data8bits & 0x80) >> 7u;

            // Rotation circulaire de (HL) vers la gauche
            mp_mpu->setMemVal(m_registers.s16bits.hl, (static_cast<std::uint8_t>((w_data8bits << 1u) & 0xFF) + m_registers.sFlags.c));
        }
        else
        {
            // Sauvegarde du LSB de (HL) dans le flag C
            m_registers.sFlags.c = (w_data8bits & 0x01);

            // Rotation ciruclaire de (HL) vers la droite
            mp_mpu->setMemVal(m_registers.s16bits.hl, (static_cast<std::uint8_t>(w_data8bits >> 1u) + static_cast<std::uint8_t>(m_registers.sFlags.c << 7u)));
        }

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(mp_mpu->getMemVal(m_registers.s16bits.hl) == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        if (w_direction == 0u)
        {
            // Sauvegarde du MSB du registre 8b dans le flag C
            m_registers.sFlags.c = (*wp_register8bits & 0x80) >> 7u;

            // Rotation circulaire du registre 8b vers la gauche
            *wp_register8bits = static_cast<std::uint8_t>((*wp_register8bits << 1u) & 0xFF) + m_registers.sFlags.c;
        }
        else
        {
            // Sauvegarde du LSB du registre 8b dans le flag C
            m_registers.sFlags.c = (*wp_register8bits & 0x01);

            // Rotation ciruclaire du registre 8b vers la droite
            *wp_register8bits = static_cast<std::uint8_t>(*wp_register8bits >> 1u) + static_cast<std::uint8_t>(m_registers.sFlags.c << 7u);
        }

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_rd_d()
{
    // Variables locales
    std::uint8_t    w_direction         = 0u;
    std::uint8_t    w_flagCSave         = 0u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du sens de rotation (0 = gauche, 1 = droite)
    w_direction = (mp_mpu->getMemVal(m_opcodeIdx) & 0x08) >> 3u;

    // Sauvegarde du flag C
    w_flagCSave = m_registers.sFlags.c;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        if (w_direction == 0u)
        {
            // Sauvegarde du MSB de (HL) dans le flag C
            m_registers.sFlags.c = (w_data8bits & 0x80) >> 7u;

            // Rotation de (HL) vers la gauche et recopie de la sauvegarde du flag C sur le LSB de (HL)
            mp_mpu->setMemVal(m_registers.s16bits.hl, (static_cast<std::uint8_t>((w_data8bits << 1u) & 0xFF) + w_flagCSave));
        }
        else
        {
            // Sauvegarde du LSB de (HL) dans le flag C
            m_registers.sFlags.c = (w_data8bits & 0x01);

            // Rotation de A vers la droite et recopie de la sauvegarde du flag C sur le MSB de A
            mp_mpu->setMemVal(m_registers.s16bits.hl, (static_cast<std::uint8_t>(w_data8bits >> 1u) + static_cast<std::uint8_t>(w_flagCSave << 7u)));
        }

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(mp_mpu->getMemVal(m_registers.s16bits.hl) == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        if (w_direction == 0u)
        {
            // Sauvegarde du MSB du registre 8b dans le flag C
            m_registers.sFlags.c = (*wp_register8bits & 0x80) >> 7u;

            // Rotation du registre 8b vers la gauche et recopie de la sauvegarde du flag C sur le LSB du registtre 8b
            *wp_register8bits = static_cast<std::uint8_t>((*wp_register8bits << 1u) & 0xFF) + w_flagCSave;
        }
        else
        {
            // Sauvegarde du LSB du registre 8b dans le flag C
            m_registers.sFlags.c = (*wp_register8bits & 0x01);

            // Rotation du registre 8b vers la droite et recopie de la sauvegarde du flag C sur le MSB du registre 8b
            *wp_register8bits = static_cast<std::uint8_t>(*wp_register8bits >> 1u) + static_cast<std::uint8_t>(w_flagCSave << 7u);
        }

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_sda_d()
{
    // Variables locales
    std::uint8_t    w_direction         = 0u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du sens de rotation (0 = gauche, 1 = droite)
    w_direction = (mp_mpu->getMemVal(m_opcodeIdx) & 0x08) >> 3u;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        if (w_direction == 0u)
        {
            // Sauvegarde du MSB de (HL) dans le flag C
            m_registers.sFlags.c = (w_data8bits & 0x80) >> 7u;

            // Rotation de (HL) vers la gauche et reset du LSB
            mp_mpu->setMemVal(m_registers.s16bits.hl, static_cast<std::uint8_t>((w_data8bits << 1u) & 0xFE));
        }
        else
        {
            // Sauvegarde du LSB de (HL) dans le flag C
            m_registers.sFlags.c = (w_data8bits & 0x01);

            // Rotation de A vers la droite et recopie MSB
            mp_mpu->setMemVal(m_registers.s16bits.hl, static_cast<std::uint8_t>((w_data8bits >> 1u) + (w_data8bits & 0x80)));
        }

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(mp_mpu->getMemVal(m_registers.s16bits.hl) == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        if (w_direction == 0u)
        {
            // Sauvegarde du MSB du registre 8b dans le flag C
            m_registers.sFlags.c = (*wp_register8bits & 0x80) >> 7u;

            // Rotation du registre 8b vers la gauche et reset du LSB
            *wp_register8bits = static_cast<std::uint8_t>((*wp_register8bits << 1u) & 0xFE);
        }
        else
        {
            // Sauvegarde du LSB du registre 8b dans le flag C
            m_registers.sFlags.c = (*wp_register8bits & 0x01);

            // Rotation du registre 8b vers la droite et recopie du MSB
            *wp_register8bits = static_cast<std::uint8_t>((*wp_register8bits >> 1u) + (*wp_register8bits & 0x80));
        }

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_swap_d()
{
    // Variables locales
    std::uint8_t    w_lsw               = 0u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Sauvegarde du LSW de (HL)
        w_lsw = w_data8bits & 0x0F;

        // Swap entre le MSW et le LSW de (HL)
        mp_mpu->setMemVal(m_registers.s16bits.hl, static_cast<std::uint8_t>(((w_data8bits & 0xF0) >> 4u) + (w_lsw << 4u)));

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(mp_mpu->getMemVal(m_registers.s16bits.hl) == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Sauvegarde du LSW du registre 8b
        w_lsw = *wp_register8bits & 0x0F;

        // Swap entre le MSW et le LSW du registre 8b
        *wp_register8bits = static_cast<std::uint8_t>(((*wp_register8bits & 0xF0) >> 4u) + (w_lsw << 4u));

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Reset des flags C, H et N
    m_registers.sFlags.c = 0u;
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_srl_d()
{
    // Variables locales
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Sauvegarde du LSB de (HL) dans le flag C
        m_registers.sFlags.c = (w_data8bits & 0x01);

        // Rotation de A vers la droite et reset MSB
        mp_mpu->setMemVal(m_registers.s16bits.hl, static_cast<std::uint8_t>((w_data8bits >> 1u) & 0x7F));

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(mp_mpu->getMemVal(m_registers.s16bits.hl) == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Sauvegarde du LSB du registre 8b dans le flag C
        m_registers.sFlags.c = (*wp_register8bits & 0x01);

        // Rotation du registre 8b vers la droite et reset du MSB
        *wp_register8bits = static_cast<std::uint8_t>((*wp_register8bits >> 1u) & 0x7F);

        // Gestion du flag Z
        m_registers.sFlags.z = static_cast<std::uint8_t>(*wp_register8bits == 0u);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Reset des flags H et N
    m_registers.sFlags.h = 0u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_bit_n_d()
{
    // Variables locales
    std::uint8_t    w_bitNumber         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    // Récupération du numéro de bit
    w_bitNumber = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;

    if (w_registerMask == 6u)
    {
        // Sauvegarde du complément du bit N de (HL) dans Z
        m_registers.sFlags.z = ~static_cast<std::uint8_t>((mp_mpu->getMemVal(m_registers.s16bits.hl) >> w_bitNumber) & 0x01);

        // Mise à jour du nombre de cycles
        m_nbCycles = 3u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Sauvegarde du complément du bit N de registre 8b dans Z
        m_registers.sFlags.z = ~static_cast<std::uint8_t>((*wp_register8bits >> w_bitNumber) & 0x01);

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Reset des flags H et N
    m_registers.sFlags.h = 1u;
    m_registers.sFlags.n = 0u;

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_res_n_d()
{
    // Variables locales
    std::uint8_t    w_bitNumber         = 0u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    // Récupération du numéro de bit
    w_bitNumber = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Reset du bit N de (HL)
        mp_mpu->setMemVal(m_registers.s16bits.hl, static_cast<std::uint8_t>(w_data8bits & ~(0x01 << w_bitNumber)));

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Reset du bit N du registre 8b
        *wp_register8bits = static_cast<std::uint8_t>(*wp_register8bits & ~(0x01 << w_bitNumber));

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Mise à jour de PC
    m_pc += 2u;
}

void Cpu::_set_n_d()
{
    // Variables locales
    std::uint8_t    w_bitNumber         = 0u;
    std::uint8_t    w_data8bits         = 0u;
    std::uint8_t 	w_registerMask      = 0u;
    std::uint8_t*	wp_register8bits    = NULL;

    // Récupération du masque du registre
    w_registerMask = mp_mpu->getMemVal(m_opcodeIdx) & 0x07;

    // Récupération du numéro de bit
    w_bitNumber = (mp_mpu->getMemVal(m_opcodeIdx) & 0x38) >> 3u;

    if (w_registerMask == 6u)
    {
        // Récupération de la valeur contenue dans (HL)
        w_data8bits = mp_mpu->getMemVal(m_registers.s16bits.hl);

        // Set du bit N de (HL)
        mp_mpu->setMemVal(m_registers.s16bits.hl, static_cast<std::uint8_t>(w_data8bits | (0x01 << w_bitNumber)));

        // Mise à jour du nombre de cycles
        m_nbCycles = 4u;
    }
    else
    {
        // Récupération du registre
        _decodeRegister8Bits(w_registerMask, wp_register8bits);

        // Set du bit N du registre 8b
        *wp_register8bits = static_cast<std::uint8_t>(*wp_register8bits | (0x01 << w_bitNumber));

        // Mise à jour du nombre de cycles
        m_nbCycles = 2u;
    }

    // Mise à jour de PC
    m_pc += 2u;
}
