#include <thread>

#include "CORE/INCLUDE/gameboy.h"

// Durée en ms du délai d'attente avant la fin effective de l'émulation
#define GB_EMULATION_END_DELAY_MS   50u


// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gameboy::Gameboy(updateScreenFunction ai_updateScreen) :
    mp_mpu(new Mpu()),
    mp_cpu(new Cpu(mp_mpu)),
    mp_gpu(new Gpu(mp_mpu, ai_updateScreen)),
    mp_timers(new Timers(mp_mpu)),
    m_isRunning(false)
{
    for (std::uint16_t w_i = 0u; w_i < MPU_BOOTSTRAP_SIZE; ++w_i)
    {
        m_romFirstBytes[w_i] = 0u;
    }

    // Initialisation des boutons
    m_directionButtons.joypadP14    = 0xF;
    m_utilityButtons.joypadP15      = 0xF;
}

// Destructeur
Gameboy::~Gameboy()
{
    delete mp_gpu;
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
    std::uint32_t 		w_i             = 0u;

    // Réinitialisation de l'émulation
    mp_mpu->initMemory();
    mp_cpu->initRegisters();

    // Ouverture du fichier en lecture
    std::ifstream w_ROMFile(ai_ROMFileName, std::ios::in | std::ifstream::binary);

    if(w_ROMFile)
    {
        // Enregistrtement des MPU_BOOTSTRAP_SIZE premiers octets
        while (w_i < MPU_BOOTSTRAP_SIZE)
        {
            w_ROMFile.get(w_caractere);
            m_romFirstBytes[w_i] = static_cast<std::uint8_t>(w_caractere);
            w_i++;
        }

        // Chargement de BANK0 et BANK1 en mémoire
        while(w_i < (MPU_MEMORY_CARD_BANK_1_OFFSET + MPU_MEMORY_CARD_BANK_SIZE))
        {
            w_ROMFile.get(w_caractere);
            mp_mpu->setMemVal((MPU_MEMORY_CARD_BANK_0_OFFSET + w_i), static_cast<std::uint8_t>(w_caractere));
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

    // Execution du bootstrap
    _executeBootstrap();

    while (m_isRunning)
    {
        _executeCycle();
    }

    return E_OK;
}

void Gameboy::stop()
{
    // Fin de l'émulation
    m_isRunning = false;

    // Temporisation pour attendre proprement la fin de l'émulation
    std::this_thread::sleep_for(std::chrono::milliseconds(GB_EMULATION_END_DELAY_MS));
}


// ********************************************************
// GESTION DES BOUTONS
// ********************************************************

void Gameboy::setPressButton(const te_button ai_button)
{
    // Parcours de la liste des boutons
    switch (ai_button)
    {
        case E_UP :
            m_directionButtons.sButtons.up = 0u;
            break;

        case E_DOWN :
            m_directionButtons.sButtons.down = 0u;
            break;

        case E_LEFT :
            m_directionButtons.sButtons.left = 0u;
            break;

        case E_RIGHT :
            m_directionButtons.sButtons.right = 0u;
            break;

        case E_A :
            m_utilityButtons.sButtons.a = 0u;
            break;

        case E_B :
            m_utilityButtons.sButtons.b = 0u;
            break;

        case E_START :
            m_utilityButtons.sButtons.start = 0u;
            break;

        case E_SELECT :
            m_utilityButtons.sButtons.select = 0u;
            break;
    }
}

void Gameboy::setReleaseButton(const te_button ai_button)
{
    // Parcours de la liste des boutons
    switch (ai_button)
    {
        case E_UP :
            m_directionButtons.sButtons.up = 1u;
            break;

        case E_DOWN :
            m_directionButtons.sButtons.down = 1u;
            break;

        case E_LEFT :
            m_directionButtons.sButtons.left = 1u;
            break;

        case E_RIGHT :
            m_directionButtons.sButtons.right = 1u;
            break;

        case E_A :
            m_utilityButtons.sButtons.a = 1u;
            break;

        case E_B :
            m_utilityButtons.sButtons.b = 1u;
            break;

        case E_START :
            m_utilityButtons.sButtons.start = 1u;
            break;

        case E_SELECT :
            m_utilityButtons.sButtons.select = 1u;
            break;
    }
}


// *****************
// Fonctions privées
// *****************

void Gameboy::_executeCycle()
{
    // Exécution des instructions entre 0x000 et 0x100
    mp_cpu->executeOpcode(mp_cpu->getRegisterPC());

    // Mise à jour de l'écran
    mp_gpu->computeScreenImage(mp_cpu->getNbCyccles());

    // Gestion des boutons
    _setButtons();

    // Gestion des interruptions
    if (mp_cpu->getRegisterIME() == 1u)
    {
        // Interruption VBLANK autorisée et demandée
        if ((mp_mpu->getMemVal(GAMEBOY_INTERRUPT_ENABLE) && GAMEBOY_VBLANK_ENABLE == 1u) && (mp_mpu->getMemVal(GAMEBOY_INTERRUPT_FLAG) && GAMEBOY_VBLANK_REQUESTED == 1u))
        {
            mp_cpu->executeInterrupt(Cpu::E_VBLANK);
        }
    }

    // Mise à jour des timers
    mp_timers->update();
}

void Gameboy::_executeBootstrap()
{
    while (m_isRunning && (mp_cpu->getRegisterPC() < MPU_BOOTSTRAP_SIZE))
    {
        _executeCycle();
    }

    if (m_isRunning)
    {
        // Chargement des 256 premiers octets de la ROM (pour remplacer le bootstrap)
        for (std::uint16_t w_i = 0u; w_i < MPU_BOOTSTRAP_SIZE; ++w_i)
        {
            mp_mpu->setMemVal(w_i, m_romFirstBytes[w_i]);
        }
    }
}

void Gameboy::_setButtons()
{
    // Récupération du type de boutons
    std::uint8_t w_buttonType = (mp_mpu->getMemVal(MPU_JOYPAD_ADDRESS) >> 4u) & 0x03;

    // Mise à jour du registre Joypad
    switch (w_buttonType)
    {
        case 1u :
            // Touches directionnelles
            mp_mpu->setJoypad(m_utilityButtons.joypadP15);
            break;

        case 2u :
            // Touches utilitaires
            mp_mpu->setJoypad(m_directionButtons.joypadP14);
            break;
    }
}
