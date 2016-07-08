#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <fstream>

#include "cpu.h"

/*! \brief	Taille de la mémoire
 *
 * La gameboy a 64 ko de mémoire.
 * Cf le mapping ci-dessous.
 */
#define CPU_MEMORY_SIZE 			65535

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

    Cpu* getCpu();

    Status loadROM(std::string ai_ROMFileName);

private:
    Cpu*            mp_cpu;                     // CPU
    std::uint8_t	m_memory[CPU_MEMORY_SIZE];	// Mémoire

    std::string     m_ROMFileName;              // Nom du fichier de la ROM à charger
};

#endif // GAMEBOY_H
