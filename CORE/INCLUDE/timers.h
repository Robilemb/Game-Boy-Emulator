#ifndef TIMERS_H
#define TIMERS_H

#include "shared_data.h"
#include "mpu.h"

class Timers
{
public:
    explicit Timers(Mpu* const aip_mpu);
    ~Timers();

    // Mise à jour des timers
    void update(const std::uint8_t ai_cpuCycles);

private:
    Mpu*            mp_mpu;         // Pointeur vers la mémoire

    std::uint8_t    m_div;          // Divider
    std::uint8_t    m_tima;         // Timer Counter
    std::uint8_t    m_tma;          // Timer Modulo
    std::uint8_t    m_tac;          // Timer Controller

    std::uint8_t    m_divCounter;   // Compteur pour incrémentation de DIV à 16kHz
    std::uint8_t    m_timaCounter;  // Compteur pour incrémentation de TIMA à la fréquence souhaité
};

#endif // TIMERS_H
