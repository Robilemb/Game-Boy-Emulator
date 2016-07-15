#include "CORE/INCLUDE/gameboy.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy()
{
    mp_cpu = new Cpu();

    // Initialisation de la mémoire
    initMemory();
}

// Destructeur
Gameboy::~Gameboy()
{
    delete mp_cpu;
}


// ********************************************************
// ACCESSEURS
// ********************************************************

Cpu* Gameboy::getCpu()
{
    return mp_cpu;
}

std::uint8_t		Gameboy::getMemVal(std::uint16_t ai_offset)
{
    return m_memory[GB_MEMORY_CARD_BANK_0_OFFSET + ai_offset];
}

void				Gameboy::setMemVal(std::uint16_t ai_offset, std::uint8_t ai_val)
{
    m_memory[GB_MEMORY_CARD_BANK_0_OFFSET + ai_offset] = ai_val;
}


// ********************************************************
// INITIALISATION DE LA MEMOIRE
// ********************************************************

void Gameboy::initMemory()
{
    // Initialisation de la mémoire
    for (std::uint16_t w_i = 0; w_i < GB_MEMORY_SIZE; w_i++)
    {
        m_memory[w_i] = 0;
    }
}


// ********************************************************
// CHARGEMENT DE LA ROM EN MEMOIRE
// ********************************************************

Status Gameboy::loadROM(std::string ai_ROMFileName)
{
    // Variables locales
    char w_caractere;
    std::uint16_t w_i = 0;

    // Réinitialisation de l'émulation
    initMemory();
    mp_cpu->initRegisters();

    // Ouverture du fichier en lecture
    std::ifstream w_ROMFile(ai_ROMFileName, std::ios::in | std::ifstream::binary);

    if(w_ROMFile)
    {
        // Chargement du contenu de la ROM dans la mémoire BANK0
        while(w_i < GB_MEMORY_CARD_BANK_0_SIZE)
        {
            w_ROMFile.get(w_caractere);

            m_memory[GB_MEMORY_CARD_BANK_0_OFFSET + w_i] = static_cast<std::uint8_t>(w_caractere);
            w_i++;
        }

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
    for (std::uint16_t w_k = 0; w_k < GB_MEMORY_CARD_BANK_0_SIZE; w_k++)
    {
        std::cout << std::hex << std::uppercase << static_cast<std::uint16_t>(m_memory[GB_MEMORY_CARD_BANK_0_OFFSET + w_k]) << " ";
    }
    std::cout << std::endl;
}


// ********************************************************
// EXECUTION DE L'EMULATION
// ********************************************************

Status Gameboy::run()
{
    // Exécution de l'opcode à l'adresse de PC
    mp_cpu->executeOpcode(&m_memory[mp_cpu->getRegisterPC()]);

    return E_OK;
}
