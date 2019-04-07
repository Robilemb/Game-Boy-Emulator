#include "CORE/INCLUDE/gpu.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gpu::Gpu(Mpu* const aip_mpu) :
    mp_mpu(aip_mpu)
{
}

// Destructeur
Gpu::~Gpu()
{
}


// ****************************************
// Calcul de l'image à afficher sur l'écran
// ****************************************

void Gpu::computeScreenImage(gbScreenImage& ai_screenImage)
{
    /*
    // Variables locales
    std::uint8_t w_tileNumber = 0u;

    // Construction du background
    for (std::uint8_t w_i = 0u; w_i < (32*32); ++w_i)
    {
        // Lecture des 32*32 numéros de tile entre 0x9800 et 0x9BFF ou 0x9C00 et 0x9FFF
        w_tileNumber = mp_mpu->getMemVal(0x9800 + static_cast<std::uint16_t>(w_i));

        // Chargement des tiles correspondantes dans m_background entre 0x8000 et 0x8FFF ou 0x8800 et 0x97FF
    }
    */

    for (std::uint8_t w_i = 0; w_i < GAMEBOY_SCREEN_WIDTH; ++w_i)
        for (std::uint8_t w_j = 0; w_j < GAMEBOY_SCREEN_HEIGHT; ++w_j)
            ai_screenImage[w_j + w_i*GAMEBOY_SCREEN_HEIGHT] = w_i;
}
