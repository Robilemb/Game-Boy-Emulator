#include "CORE/INCLUDE/gameboy.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy()
{
    mp_cpu = new Cpu();

    // Initialisation de la mémoire
    for (std::uint16_t w_i = 0; w_i < CPU_MEMORY_SIZE; w_i++)
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
    // Ouverture du fichier en lecture
    std::ifstream w_ROMFile(ai_ROMFileName, std::ios::in);

    if(w_ROMFile)
    {
        // Chargement de la ROM en mémoire
        char caractere;
        while(w_ROMFile.get(caractere))
        {
            std::cout << std::hex << std::uppercase << (std::uint16_t)static_cast<std::uint8_t>(caractere) << " ";
        }
        std::cout << std::endl;

        std::cout << "FIN DE LA LECTURE" << std::endl;

        // Fermeture du fichier
        w_ROMFile.close();
    }
    else
    {
        return E_ERROR;
    }

    return E_OK;
}
