#ifndef MPU_H
#define MPU_H

#include <iostream>

// Taille de la mémoire = 64ko
#define MPU_MEMORY_SIZE                 65535

// Offset mémoire pour accès à la cartouche
#define MPU_MEMORY_CARD_BANK_0_OFFSET   0

// MBC (Memory Bank Controller) type 0 : pas de ROM banking, taille max de la ROM = 32ko
#define MPU_MEMORY_CARD_MBC_0_SIZE      32767

class Mpu
{
public:
    explicit Mpu();
    ~Mpu();

    // Initialisation de la mémoire
    void initMemory();

    // Accesseurs sur la mémoire
    std::uint8_t getMemVal(std::uint16_t ai_offset) const;
    void setMemVal(std::uint16_t ai_offset, std::uint8_t ai_val);


private:
    std::uint8_t m_memory[MPU_MEMORY_SIZE];	// Mémoire
};

#endif // MPU_H
