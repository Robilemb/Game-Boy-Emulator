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

    // Chargement de la ROM en mémoire
    te_status loadROM(const std::string& ai_ROMFileName);

    // Exécution de l'émulation
    te_status run();

    // Accesseur sur le CPU
    const Cpu* getCpu() const;

    // Accesseur sur la MPU
    const Mpu* getMpu() const;

private:
    Cpu*            	mp_cpu;     // CPU
    Mpu*                mp_mpu;     // MPU
};

#endif // GAMEBOY_H
