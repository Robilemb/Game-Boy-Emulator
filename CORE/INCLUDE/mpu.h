#ifndef MPU_H
#define MPU_H

#include <iostream>
#include <array>

// Taille de la mémoire = 64ko
#define MPU_MEMORY_SIZE                 65535u

// Taille d'une bank
#define MPU_MEMORY_CARD_BANK_SIZE       0x4000

// Offsets mémoire pour accès aux différentes banks de la cartouche
#define MPU_MEMORY_CARD_BANK_0_OFFSET   0x0000
#define MPU_MEMORY_CARD_BANK_1_OFFSET   0x4000

#define MPU_BOOTSTRAP_SIZE              256u

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
    std::array<std::uint8_t, MPU_MEMORY_SIZE> m_memory;     // Mémoire
};

#endif // MPU_H
