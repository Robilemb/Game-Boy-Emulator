#ifndef MPU_H
#define MPU_H

#include <iostream>
#include <array>

// Taille de la mémoire = 64ko
#define MPU_MEMORY_SIZE                 65536u

// Taille d'une bank
#define MPU_MEMORY_CARD_BANK_SIZE       0x4000

// Offsets mémoire pour accès aux différentes banks de la cartouche
#define MPU_MEMORY_CARD_BANK_0_OFFSET   0x0000
#define MPU_MEMORY_CARD_BANK_1_OFFSET   0x4000

// OAM
#define MPU_OAM_START_ADDRESS           0xFE00
#define MPU_OAM_SIZE                    0xA0

// Registre du Joypad
#define MPU_JOYPAD_ADDRESS              0xFF00

// Registres des timers
#define MPU_DIV_ADDRESS                 0xFF04
#define MPU_TIMA_ADDRESS                0xFF05
#define MPU_TMA_ADDRESS                 0xFF06
#define MPU_TAC_ADDRESS                 0xFF07

// Registre DMA
#define MPU_DMA_ADDRESS                 0xFF46

// Registres de controle des interruptions
#define MPU_IF_ADDRESS                  0xFF0F
#define MPU_IE_ADDRESS                  0xFFFF

#define MPU_BOOTSTRAP_SIZE              256u

class Mpu
{
public:
    explicit Mpu();
    ~Mpu();

    // Initialisation de la mémoire
    void initMemory();

    // Chargement d'une ROM
    void setROMData(const std::uint16_t ai_offset, const std::uint8_t ai_val);

    // Accesseurs sur la mémoire
    std::uint8_t getMemVal(const std::uint16_t ai_offset) const;
    void setMemVal(const std::uint16_t ai_offset, const std::uint8_t ai_val);

    // Set du registre Joypad
    void setJoypad(const std::uint8_t ai_joypad);

    // Set du registre Divider
    void setDivider(const std::uint8_t ai_divider);


private:
    // Déclenche un transfert DMA
    void _dma_transfert(const std::uint8_t ai_startAddress);


private:
    std::array<std::uint8_t, MPU_MEMORY_SIZE> m_memory;     // Mémoire
};

#endif // MPU_H
