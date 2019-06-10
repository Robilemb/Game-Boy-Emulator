#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>
#include <fstream>
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
    // Execution d'un cycle d'horloge
    void _executeCycle();

    // Execution du bootstrap
    void _executeBootstrap();

private:
    Mpu*                                            mp_mpu;             // MPU
    Cpu*                                            mp_cpu;             // CPU
    Gpu*                                            mp_gpu;             // GPU

    bool                                            m_isRunning;        // Booléen indiquant si l'émulation est en cours d'exécution    

    std::array<std::uint8_t, MPU_BOOTSTRAP_SIZE>    m_romFirstBytes;    // MPU_BOOTSTRAP_SIZE premiers octets de la ROM à recharger après l'exécution du bootstrap
};

#endif // GAMEBOY_H
