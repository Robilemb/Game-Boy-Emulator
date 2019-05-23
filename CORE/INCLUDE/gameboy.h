#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>
#include <fstream>
#include <functional>
#include <array>
#include <chrono>

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
    // Execution du bootstrap
    void _executeBootstrap();

    // Raffraichissement de l'écran
    void _setScreen();

private:
    Mpu*                                            mp_mpu;             // MPU
    Cpu*                                            mp_cpu;             // CPU
    Gpu*                                            mp_gpu;             // GPU

    std::chrono::high_resolution_clock::time_point  m_gpuClock;         // Horloge pour le raffraichissement de l'écran
    gbScreenImage                                   m_screenImage;      // Image à afficher sur l'écran
    updateScreenFunction                            updateScreen;       // Callback de mise à jour de l'écran

    bool                                            m_isRunning;        // Booléen indiquant si l'émulation est en cours d'exécution

    std::array<std::uint8_t, MPU_BOOTSTRAP_SIZE>    m_romFirstBytes;    // MPU_BOOTSTRAP_SIZE premiers octets de la ROM à recharger après l'exécution du bootstrap
};

#endif // GAMEBOY_H
