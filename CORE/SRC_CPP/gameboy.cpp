#include "CORE/INCLUDE/gameboy.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy()
{
    mp_mpu      = new Mpu();
    mp_cpu      = new Cpu(mp_mpu);

    m_isRunning = false;
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

const Cpu* Gameboy::getCpu() const
{
    return mp_cpu;
}

const Mpu* Gameboy::getMpu() const
{
    return mp_mpu;
}


// ********************************************************
// CHARGEMENT DE LA ROM EN MEMOIRE
// ********************************************************

te_status Gameboy::loadROM(const std::string& ai_ROMFileName)
{
    // Variables locales
    char 				w_caractere;
    std::uint32_t 		w_i = 0u;

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
// EXECUTION DE L'EMULATION
// ********************************************************

te_status Gameboy::start()
{
    // Démarrage de l'émulation
    m_isRunning = true;

    while (m_isRunning)
    {
        // Exécution de l'opcode à l'adresse de PC
        mp_cpu->executeOpcode(mp_cpu->getRegisterPC());
    }

    return E_OK;
}

void Gameboy::stop()
{
    // Fin de l'émulation
    m_isRunning = false;
}
