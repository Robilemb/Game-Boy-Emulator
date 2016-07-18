#include "CORE/INCLUDE/gameboy.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy()
{
    mp_mpu = new Mpu();
    mp_cpu = new Cpu(mp_mpu);

    m_romSize = 0u;
}

// Destructeur
Gameboy::~Gameboy()
{
    delete mp_cpu;
    delete mp_mpu;
}


// ********************************************************
// ACCESSEURS
// ********************************************************

Cpu* Gameboy::getCpu()
{
    return mp_cpu;
}

Mpu* Gameboy::getMpu()
{
    return mp_mpu;
}


// ********************************************************
// CHARGEMENT DE LA ROM EN MEMOIRE
// ********************************************************

Status Gameboy::loadROM(std::string ai_ROMFileName)
{
    // Variables locales
    char 				w_caractere;
    std::uint32_t 		w_i = 0;

    // Réinitialisation de l'émulation
    mp_mpu->initMemory();
    mp_cpu->initRegisters();

    // Ouverture du fichier en lecture
    std::ifstream w_ROMFile(ai_ROMFileName, std::ios::in | std::ifstream::binary);

    if(w_ROMFile)
    {
        // Chargement du contenu de la ROM dans la mémoire BANK0
        while(w_i < MPU_MEMORY_CARD_MBC_0_SIZE)
        {
            w_ROMFile.get(w_caractere);

            mp_mpu->setMemVal(MPU_MEMORY_CARD_BANK_0_OFFSET+w_i, static_cast<std::uint8_t>(w_caractere));
            w_i++;
        }

        m_romSize = w_i;

        // Fermeture du fichier
        w_ROMFile.close();
    }
    else
    {
        return E_ERROR;
    }

    return E_OK;
}


// ********************************************************
// AFFICHAGE DE LA ROM
// ********************************************************

void Gameboy::printROMBank0()
{
    // Affichage de la ROM BANK 0
    for (std::uint16_t w_k = 0; w_k < MPU_MEMORY_CARD_MBC_0_SIZE; w_k++)
    {
        std::cout << std::hex << std::uppercase << static_cast<std::uint16_t>(mp_mpu->getMemVal(MPU_MEMORY_CARD_BANK_0_OFFSET+w_k)) << " ";
    }
    std::cout << std::endl;
}

std::uint32_t		Gameboy::getRomSize()
{
	return m_romSize;
}

std::string			Gameboy::showInstr(std::uint16_t ai_pos)
{
    return mp_cpu->showInstruction(ai_pos);
}


// ********************************************************
// EXECUTION DE L'EMULATION
// ********************************************************

void    Gameboy::execInstr(std::uint16_t ai_pos)
{
    mp_cpu->executeOpcode(ai_pos);
}

Status Gameboy::run()
{
    // Exécution de l'opcode à l'adresse de PC
    execInstr(mp_cpu->getRegisterPC());

    return E_OK;
}
