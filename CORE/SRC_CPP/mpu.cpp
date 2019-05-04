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
    for (std::uint16_t w_i = 0u; w_i < MPU_MEMORY_SIZE; ++w_i)
    {
        m_memory[w_i] = 0u;
    }

    // Temporaire
    m_memory[0x8000] = 0x3C;
    m_memory[0x8001] = 0x3C;
    m_memory[0x8002] = 0x42;
    m_memory[0x8003] = 0x7E;
    m_memory[0x8004] = 0x85;
    m_memory[0x8005] = 0xFB;
    m_memory[0x8006] = 0x81;
    m_memory[0x8007] = 0xFF;
    m_memory[0x8008] = 0xA1;
    m_memory[0x8009] = 0xDF;
    m_memory[0x800A] = 0xB1;
    m_memory[0x800B] = 0xCF;
    m_memory[0x800C] = 0x42;
    m_memory[0x800D] = 0x7E;
    m_memory[0x800E] = 0x3C;
    m_memory[0x800F] = 0x3C;

    m_memory[0x8010] = 0x00;
    m_memory[0x8011] = 0x00;
    m_memory[0x8012] = 0x00;
    m_memory[0x8013] = 0x00;
    m_memory[0x8014] = 0xFF;
    m_memory[0x8015] = 0x00;
    m_memory[0x8016] = 0xFF;
    m_memory[0x8017] = 0x00;
    m_memory[0x8018] = 0x00;
    m_memory[0x8019] = 0xFF;
    m_memory[0x801A] = 0x00;
    m_memory[0x801B] = 0xFF;
    m_memory[0x801C] = 0xFF;
    m_memory[0x801D] = 0xFF;
    m_memory[0x801E] = 0xFF;
    m_memory[0x801F] = 0xFF;

    for (int w_i = 0; w_i < (32*32); ++w_i)
        m_memory[0x9800 + w_i] = (w_i%2u);
}
