#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <cstdint>

#include <fstream>

#include "cpu.h"

/*! \brief	Taille de la mémoire
 *
 * La gameboy a 64 ko de mémoire.
 * Cf le mapping ci-dessous.
 */
#define GB_MEMORY_SIZE                  65535

// Offset mémoire pour accès à la cartouche
#define GB_MEMORY_CARD_BANK_0_OFFSET    0

// Nombre d'octet en mémoire de la cartouche (32 ko)
#define GB_MEMORY_CARD_BANK_0_SIZE      16383

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

    // Accesseurs sur la mémoire
    std::uint8_t getMemVal(std::uint16_t ai_offset);
    void setMemVal(std::uint16_t ai_offset, std::uint8_t ai_val);

    // Chargement de la ROM en mémoire
    Status loadROM(std::string ai_ROMFileName);

    // Affichage de la ROM BANK 0
    void printROMBank0();

    // Exécution de l'émulation
    Status run();

public:
    std::uint32_t		getRomSize();
    std::uint8_t		getMemVal(std::uint32_t ai_offset);
    void				setMemVal(std::uint32_t ai_offset, std::uint8_t ai_val);

    std::string			showInstr(std::uint16_t ai_pos);
    void				execInstr(std::uint16_t ai_pos);

    // Initialisation de la mémoire
    void 				initMemory();

private:
    Cpu*            	mp_cpu;                     // CPU
    std::uint8_t		m_memory[GB_MEMORY_SIZE];	// Mémoire
    std::uint32_t		m_romSize;
};

#endif // GAMEBOY_H
