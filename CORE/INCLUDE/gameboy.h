#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>
#include <fstream>
#include <functional>
#include <array>

#include "shared_data.h"
#include "cpu.h"
#include "gpu.h"
#include "mpu.h"

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
    typedef std::function<void(const gbScreenImage&)> updateScreenFunction;

    explicit Gameboy(updateScreenFunction ai_updateScreen);
    ~Gameboy();

    // Chargement de la ROM en mémoire
    te_status loadROM(const std::string& ai_ROMFileName);

    // Exécution de l'émulation
    te_status start();

    // Arret de l'émulation
    void stop();

    // Accesseur sur le CPU
    const Cpu* getCpu() const;

    // Accesseur sur la MPU
    const Mpu* getMpu() const;

private:
    Mpu*                    mp_mpu;         // MPU
    Cpu*                    mp_cpu;         // CPU
    Gpu*                    mp_gpu;         // GPU

    gbScreenImage           m_screenImage;  // Image à afficher sur l'écran
    updateScreenFunction    updateScreen;   // Callback de mise à jour de l'écran

    bool                    m_isRunning;    // Booléen indiquant si l'émulation est en cours d'exécution
};

#endif // GAMEBOY_H
