#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>
#include <fstream>
#include <array>

#include "shared_data.h"
#include "cpu.h"
#include "gpu.h"
#include "mpu.h"
#include "timers.h"

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
    // Enuméré des différents boutons
    enum te_button
    {
        E_UP    = 0u,
        E_DOWN,
        E_LEFT,
        E_RIGHT,
        E_A,
        E_B,
        E_START,
        E_SELECT
    };

public:
    explicit Gameboy(updateScreenFunction ai_updateScreen);
    ~Gameboy();

    // Chargement de la ROM en mémoire
    te_status loadROM(const std::string& ai_ROMFileName);

    // Exécution de l'émulation
    te_status start();

    // Arret de l'émulation
    void stop();

    // Bouton pressé
    void setPressButton(const te_button ai_button);

    // Bouton relaché
    void setReleaseButton(const te_button ai_button);

    // Accesseur sur le CPU
    const Cpu* getCpu() const;

    // Accesseur sur la MPU
    const Mpu* getMpu() const;

private:
    // Union contenant l'état des boutons de direction (0 = pressé ; 1 sinon)
    union tu_directionButtons
    {
        struct ts_buttons
        {
            std::uint8_t    right:1u,
                            left:1u,
                            up:1u,
                            down:1u;
        } sButtons;

        std::uint8_t joypadP14;
    };

    // Union contenant l'état des boutons utilitaires (0 = pressé ; 1 sinon)
    union tu_utilityButtons
    {
        struct ts_buttons
        {
            std::uint8_t    a:1u,
                            b:1u,
                            select:1u,
                            start:1u;
        } sButtons;

        std::uint8_t joypadP15;
    };

private:
    // Execution d'un cycle d'horloge
    void _executeCycle();

    // Execution du bootstrap
    void _executeBootstrap();

    // Gestion des boutons
    void _setButtons();

private:
    Mpu*                                            mp_mpu;             // MPU
    Cpu*                                            mp_cpu;             // CPU
    Gpu*                                            mp_gpu;             // GPU
    Timers*                                         mp_timers;          // Timers

    bool                                            m_isRunning;        // Booléen indiquant si l'émulation est en cours d'exécution    

    std::array<std::uint8_t, MPU_BOOTSTRAP_SIZE>    m_romFirstBytes;    // MPU_BOOTSTRAP_SIZE premiers octets de la ROM à recharger après l'exécution du bootstrap

    tu_directionButtons                             m_directionButtons; // Etat des boutons de direction (0 = pressé ; 1 sinon)
    tu_utilityButtons                               m_utilityButtons;   // Etat des boutons de utilitaire (0 = pressé ; 1 sinon)
};

#endif // GAMEBOY_H
