#include "CORE/INCLUDE/timers.h"

#define TIMERS_DIV_INC_MOD          16u     // 16KHz

#define TIMERS_TAC_CLOCK_0_INC_MOD  64u     // 256u    // 4KHz
#define TIMERS_TAC_CLOCK_1_INC_MOD  1u      // 4u      // 256KHz
#define TIMERS_TAC_CLOCK_2_INC_MOD  4u      // 16u     // 64KHz
#define TIMERS_TAC_CLOCK_3_INC_MOD  16u     // 64u     // 16KHz


// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Timers::Timers(Mpu* const aip_mpu) :
    mp_mpu(aip_mpu),
    m_div(0u),
    m_tima(0u),
    m_divCounter(0u),
    m_timaCounter(0u)
{
    // Initialisation des registres
    mp_mpu->setMemVal(MPU_DIV_ADDRESS,   0u);
    mp_mpu->setMemVal(MPU_TIMA_ADDRESS,  0u);
    mp_mpu->setMemVal(MPU_TMA_ADDRESS,   0u);
    mp_mpu->setMemVal(MPU_TAC_ADDRESS,   0u);
}

// Destructeur
Timers::~Timers()
{
}


// ********************************************************
// Mise à jour des timers
// ********************************************************

void Timers::update(const std::uint8_t ai_cpuCycles)
{
    // Variable locale
    std::uint16_t w_modulo = 0u;

    // Valeur du registre IF
    std::uint8_t w_ifRegister = mp_mpu->getMemVal(MPU_IF_ADDRESS);

    // Mise à jour du compteur de DIV
    m_divCounter += ai_cpuCycles;

    // Mise à jour de DIV
    if (m_divCounter % TIMERS_DIV_INC_MOD == 0u)
    {
        m_div++;
        mp_mpu->setDivider(m_div);

        // Reset du compteur
        m_divCounter = 0u;
    }

    // Si le timer est actif
    if ((mp_mpu->getMemVal(MPU_TAC_ADDRESS) & 0x04) == 0x04)
    {
        // Mise à jour du compteur de TIMA
        m_timaCounter += ai_cpuCycles;

        // Récupération du modulo
        switch (mp_mpu->getMemVal(MPU_TAC_ADDRESS) & 0x03)
        {
            case 0u:
                w_modulo = TIMERS_TAC_CLOCK_0_INC_MOD;
                break;

            case 1u:
                w_modulo = TIMERS_TAC_CLOCK_1_INC_MOD;
                break;

            case 2u:
                w_modulo = TIMERS_TAC_CLOCK_2_INC_MOD;
                break;

            case 3u:
                w_modulo = TIMERS_TAC_CLOCK_3_INC_MOD;
                break;

            default :
                std::cout << "Erreur : Horloge inconnu, mise à jour du timer impossible." << std::endl;
                exit(-1);
        }

        // Mise à jour de TIMA
        if (m_timaCounter % w_modulo == 0u)
        {
            // Gestion de l'overflow du timer
            if (m_tima == 0xFF)
            {
                // Chargement du registre TMA
                m_tima = mp_mpu->getMemVal(MPU_TMA_ADDRESS);

                // Demande d'interruption TIMER
                mp_mpu->setMemVal(MPU_IF_ADDRESS, (w_ifRegister | GAMEBOY_TIMER_REQUESTED));
            }
            else
            {
                // Incrémentation du timer
                m_tima++;
            }

            // Reset du compteur
            m_timaCounter = 0u;
        }

        // Ecriture en mémoire
        mp_mpu->setMemVal(MPU_TIMA_ADDRESS, m_tima);
    }
}
