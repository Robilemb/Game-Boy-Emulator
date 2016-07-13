#include "CORE/INCLUDE/gameboy.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy()
{
    mp_cpu = new Cpu();

    // Initialisation de la mémoire
    for (std::uint16_t w_i = 0; w_i < GB_MEMORY_SIZE; w_i++)
    {
        m_memory[w_i] = 0;
    }
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


// ********************************************************
// CHARGEMENT DE LA ROM EN MEMOIRE
// ********************************************************

Status Gameboy::loadROM(std::string ai_ROMFileName)
{
    // Variables locales
    char w_caractere;
    std::uint16_t w_i = 0;

    // Ouverture du fichier en lecture
    std::ifstream w_ROMFile(ai_ROMFileName, std::ios::in);

    if(w_ROMFile)
    {
        // Chargement du contenu de la ROM dans la mémoire
        while((w_ROMFile.get(w_caractere)) || (w_i < GB_MEMORY_CARD_BANK_0_SIZE))
        {
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
