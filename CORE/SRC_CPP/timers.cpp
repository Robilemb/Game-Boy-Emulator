#include "CORE/INCLUDE/timers.h"

#define TIMERS_DIV_INC_MOD          64u     // = 1MHz/16KHz

#define TIMERS_TAC_CLOCK_0_INC_MOD  256u    // = 1MHz/4KHz
#define TIMERS_TAC_CLOCK_1_INC_MOD  4u      // = 1MHz/256KHz
#define TIMERS_TAC_CLOCK_2_INC_MOD  16u     // = 1MHz/64KHz
#define TIMERS_TAC_CLOCK_3_INC_MOD  64u     // = 1MHz/16KHz


// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Timers::Timers(Mpu* const aip_mpu) :
    mp_mpu(aip_mpu),
    m_div(0u),
    m_tima(0u)
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

void Timers::update()
{
    // Mise à jour du diviser
    m_div = (m_div + 1u) % TIMERS_DIV_INC_MOD;
    mp_mpu->setDivider(m_div);

    // Si le timer est actif
    if ((mp_mpu->getMemVal(MPU_TAC_ADDRESS) & 0x04) == 0x04)
    {
        // Gestion de l'overflow du timer
        if ((static_cast<std::uint16_t>(m_tima) + 1u) > 0xFF)
        {
            // Chargement du registre TMA
            m_tima = mp_mpu->getMemVal(MPU_TMA_ADDRESS);
        }
        else
        {
            // Incrémentation du timer
            m_tima += 1u;

            // Reset en fonction de l'horloge selectionnee
            switch (mp_mpu->getMemVal(MPU_TAC_ADDRESS) & 0x03)
            {
                case 0u:
                    // Automatique via le type du timer
                    break;

                case 1u:
                    m_tima %= TIMERS_TAC_CLOCK_1_INC_MOD;
                    break;

                case 2u:
                    m_tima %= TIMERS_TAC_CLOCK_2_INC_MOD;
                    break;

                case 3u:
                    m_tima %= TIMERS_TAC_CLOCK_3_INC_MOD;
                    break;

                default :
                    std::cout << "Erreur : Horloge inconnu, mise à jour du timer impossible." << std::endl;
                    exit(-1);
            }
        }
    }

    // Mise à jour du timer TIMA
    mp_mpu->setMemVal(MPU_TIMA_ADDRESS, m_tima);
}
