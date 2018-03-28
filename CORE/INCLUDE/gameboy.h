#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>
#include <fstream>

#include "cpu.h"
#include "mpu.h"

// Enum des status de fonction
enum te_status
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
    const Cpu* getCpu() const;

    // Accesseur sur la MPU
    const Mpu* getMpu() const;

    // Chargement de la ROM en mémoire
    te_status loadROM(const std::string& ai_ROMFileName);

    // Affichage de la ROM BANK 0
    void printROMBank0() const;

    // Exécution de l'émulation
    te_status run();

public:
    std::uint32_t		getRomSize() const;

    std::string			showInstr(std::uint16_t ai_pos) const;
    void				execInstr(std::uint16_t ai_pos);

private:
    Cpu*            	mp_cpu;     // CPU
    Mpu*                mp_mpu;     // MPU
    std::uint32_t		m_romSize;  // Taille de la ROM
};

#endif // GAMEBOY_H
