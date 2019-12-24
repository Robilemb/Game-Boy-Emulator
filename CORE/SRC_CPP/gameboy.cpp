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
    m_isRunning(false),
    m_debug(false)
{
    for (std::uint16_t w_i = 0u; w_i < MPU_BOOTSTRAP_SIZE; ++w_i)
    {
        m_romFirstBytes[w_i] = 0u;
    }
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
            mp_mpu->setROMData((MPU_MEMORY_CARD_BANK_0_OFFSET + w_i), static_cast<std::uint8_t>(w_caractere));
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
    // Valeur du registre IF
    std::uint8_t w_ifRegister = mp_mpu->getMemVal(MPU_IF_ADDRESS);

    // Parcours de la liste des boutons
    switch (ai_button)
    {
        case E_UP :
            mp_mpu->setJoypadUp(true);
            break;

        case E_DOWN :
            mp_mpu->setJoypadDown(true);
            break;

        case E_LEFT :
            mp_mpu->setJoypadLeft(true);
            break;

        case E_RIGHT :
            mp_mpu->setJoypadRight(true);
            break;

        case E_A :
            mp_mpu->setJoypadA(true);
            break;

        case E_B :
            mp_mpu->setJoypadB(true);
            break;

        case E_START :
            mp_mpu->setJoypadStart(true);
            break;

        case E_SELECT :
            mp_mpu->setJoypadSelect(true);
            break;
    }

    // Demande d'interruption JOYPAD
    mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister | GAMEBOY_JOYPAD_REQUESTED));
}

void Gameboy::setReleaseButton(const te_button ai_button)
{
    // Parcours de la liste des boutons
    switch (ai_button)
    {
        case E_UP :
            mp_mpu->setJoypadUp(false);
            break;

        case E_DOWN :
            mp_mpu->setJoypadDown(false);
            break;

        case E_LEFT :
            mp_mpu->setJoypadLeft(false);
            break;

        case E_RIGHT :
            mp_mpu->setJoypadRight(false);
            break;

        case E_A :
            mp_mpu->setJoypadA(false);
            break;

        case E_B :
            mp_mpu->setJoypadB(false);
            break;

        case E_START :
            mp_mpu->setJoypadStart(false);
            break;

        case E_SELECT :
            mp_mpu->setJoypadSelect(false);
            break;
    }
}


// *****************
// Fonctions privées
// *****************

void Gameboy::_executeCycle()
{
    // Exécution d'une instruction CPU
    mp_cpu->executeOpcode(mp_cpu->getRegisterPC());

    // Mise à jour de l'écran
    mp_gpu->computeScreenImage(mp_cpu->getNbCyccles());

    // Gestion des interruptions
    if (mp_cpu->getRegisterIME() == 1u)
    {
        // Interruption VBLANK autorisée et demandée
        if (((mp_mpu->getMemVal(MPU_IE_ADDRESS) & GAMEBOY_VBLANK_ENABLE) == GAMEBOY_VBLANK_ENABLE) && ((mp_mpu->getMemVal(MPU_IF_ADDRESS) & GAMEBOY_VBLANK_REQUESTED) == GAMEBOY_VBLANK_REQUESTED))
        {
            mp_cpu->executeInterrupt(Cpu::E_VBLANK);
        }

        // Interruption LCD_STAT autorisée et demandée
        if (((mp_mpu->getMemVal(MPU_IE_ADDRESS) & GAMEBOY_LCD_STAT_ENABLE) == GAMEBOY_LCD_STAT_ENABLE) && ((mp_mpu->getMemVal(MPU_IF_ADDRESS) & GAMEBOY_LCD_STAT_REQUESTED) == GAMEBOY_LCD_STAT_REQUESTED))
        {
            mp_cpu->executeInterrupt(Cpu::E_LCD_STAT);
        }

        // Interruption TIMER autorisée et demandée
        if (((mp_mpu->getMemVal(MPU_IE_ADDRESS) & GAMEBOY_TIMER_ENABLE) == GAMEBOY_TIMER_ENABLE) && ((mp_mpu->getMemVal(MPU_IF_ADDRESS) & GAMEBOY_TIMER_REQUESTED) == GAMEBOY_TIMER_REQUESTED))
        {
            mp_cpu->executeInterrupt(Cpu::E_TIMER);
        }

        // Interruption SERIAL autorisée et demandée
        if (((mp_mpu->getMemVal(MPU_IE_ADDRESS) & GAMEBOY_SERIAL_ENABLE) == GAMEBOY_SERIAL_ENABLE) && ((mp_mpu->getMemVal(MPU_IF_ADDRESS) & GAMEBOY_SERIAL_REQUESTED) == GAMEBOY_SERIAL_REQUESTED))
        {
            mp_cpu->executeInterrupt(Cpu::E_SERIAL);
        }

        // Interruption JOYPAD autorisée et demandée
        if (((mp_mpu->getMemVal(MPU_IE_ADDRESS) & GAMEBOY_JOYPAD_ENABLE) == GAMEBOY_JOYPAD_ENABLE) && ((mp_mpu->getMemVal(MPU_IF_ADDRESS) & GAMEBOY_JOYPAD_REQUESTED) == GAMEBOY_JOYPAD_REQUESTED))
        {
            mp_cpu->executeInterrupt(Cpu::E_JOYPAD);
        }
    }

    // Mise à jour des timers
    mp_timers->update(mp_cpu->getNbCyccles());
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
            mp_mpu->setROMData(w_i, m_romFirstBytes[w_i]);
        }
    }
}

void Gameboy::setDebug()
{
    if (m_debug == true) m_debug = false;
    else m_debug = true;
}
