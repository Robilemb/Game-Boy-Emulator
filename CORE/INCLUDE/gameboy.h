#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>
#include <fstream>
#include <functional>
#include <array>

#include "cpu.h"
#include "mpu.h"

#define GAMEBOY_SCREEN_WIDTH    160
#define GAMEBOY_SCREEN_HEIGHT   144
#define GAMEBOY_SCREEN_SIZE     (GAMEBOY_SCREEN_WIDTH*GAMEBOY_SCREEN_HEIGHT)

// Enum des status de fonction
enum te_status
{
    E_UNDEFINED = 0u,
    E_OK,
    E_ERROR
};

class Gameboy
{
public:
    //typedef std::function<void(const std::array<std::uint8_t, GAMEBOY_SCREEN_SIZE>&)> updateScreenFunction;
    typedef std::function<void(const std::string&)> updateScreenFunction;

    explicit Gameboy();
    ~Gameboy();

    // Chargement de la ROM en mémoire
    te_status loadROM(const std::string& ai_ROMFileName);

    // Exécution de l'émulation
    te_status start(updateScreenFunction updateScreen);

    // Arret de l'émulation
    void stop();

    // Accesseur sur le CPU
    const Cpu* getCpu() const;

    // Accesseur sur la MPU
    const Mpu* getMpu() const;

private:
    Cpu*                    mp_cpu;         // CPU
    Mpu*                    mp_mpu;         // MPU

    bool                    m_isRunning;    // Booléen indiquant si l'émulation est en cours d'exécution
};

#endif // GAMEBOY_H
