#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>

#include <fstream>

#include "cpu.h"
#include "mpu.h"

// Enum des status de fonction
enum Status
{
    E_UNDEFINED = 0,
    E_OK,
    E_ERROR
};

class Gameboy
{
public:
    explicit Gameboy();
    ~Gameboy();

    // Accesseur sur le CPU
    Cpu* getCpu();

    // Accesseur sur la MPU
    Mpu* getMpu();

    // Chargement de la ROM en mémoire
    Status loadROM(std::string ai_ROMFileName);

    // Affichage de la ROM BANK 0
    void printROMBank0();

    // Exécution de l'émulation
    Status run();

public:
    std::uint32_t		getRomSize();

    std::string			showInstr(std::uint16_t ai_pos);
    void				execInstr(std::uint16_t ai_pos);

private:
    Cpu*            	mp_cpu;     // CPU
    Mpu*                mp_mpu;     // MPU
    std::uint32_t		m_romSize;  // Taille de la ROM
};

#endif // GAMEBOY_H
