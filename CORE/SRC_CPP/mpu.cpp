#include "CORE/INCLUDE/mpu.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

Mpu::Mpu()
{
    // Initialisation de la mémoire
    initMemory();
}

Mpu::~Mpu()
{
}


// ********************************************************
// ACCESSEURS
// ********************************************************

std::uint8_t Mpu::getMemVal(std::uint16_t ai_offset) const
{
    return m_memory[ai_offset];
}

void Mpu::setMemVal(std::uint16_t ai_offset, std::uint8_t ai_val)
{
    m_memory[ai_offset] = ai_val;
}


// ********************************************************
// INITIALISATION DE LA MEMOIRE
// ********************************************************

void Mpu::initMemory()
{
    // Initialisation de la mémoire
    for (std::uint16_t w_i = 0; w_i < MPU_MEMORY_SIZE; w_i++)
    {
        m_memory[w_i] = 0;
    }
}
