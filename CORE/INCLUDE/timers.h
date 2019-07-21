#ifndef TIMERS_H
#define TIMERS_H

#include "mpu.h"

class Timers
{
public:
    explicit Timers(Mpu* const aip_mpu);
    ~Timers();

    // Mise à jour des timers
    void update();

private:
    Mpu*            mp_mpu;     // Pointeur vers la mémoire

    std::uint8_t    m_div;      // Divider
    std::uint8_t    m_tima;     // Timer Counter
    std::uint8_t    m_tma;      // Timer Modulo
    std::uint8_t    m_tac;      // Timer Controller
};

#endif // TIMERS_H
