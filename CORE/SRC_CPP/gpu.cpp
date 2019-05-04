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

void Gpu::computeScreenImage(gbScreenImage& ao_screenImage)
{
    // Variables locales
    std::uint8_t w_tileNumber = 0u;
    std::uint8_t w_tileData[16];
    std::uint8_t w_pixelLsb     = 0u;
    std::uint8_t w_pixelMsb     = 0u;
    std::uint8_t w_pixelValue   = 0u;
    std::uint8_t w_pixelColor   = 0u;

    // Chargement des tiles correspondantes dans m_background entre 0x8000 et 0x8FFF ou 0x8800 et 0x97FF
    for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_TILE_WIDTH; ++w_i)
    {
        for (std::uint8_t w_j = 0u; w_j < GPU_BACKGROUND_TILE_HEIGHT; ++w_j)
        {
            // Numéro de tile à charger
            w_tileNumber = mp_mpu->getMemVal(0x9800 + w_j + (w_i*GPU_BACKGROUND_TILE_HEIGHT));

            for (std::uint8_t w_k = 0u; w_k < GPU_TILE_NB_BYTES; ++w_k)
            {
                // Récupération des octets de représentation de la tile
                w_tileData[w_k] = mp_mpu->getMemVal(0x8000 + (GPU_TILE_NB_BYTES*w_tileNumber) + w_k);
            }

            // Conversion en couleur
            for (std::uint8_t w_x = 0u; w_x < GPU_TILE_WIDTH; ++w_x)
            {
                for (std::uint8_t w_y = 0u; w_y < GPU_TILE_HEIGHT; ++w_y)
                {
                    // Lecture de la valeur du pixel verticalement de haut en bas
                    w_pixelMsb      = (w_tileData[(2u*w_y)+1u] & (1u << (GPU_TILE_WIDTH-w_x-1u))) >> (GPU_TILE_WIDTH-w_x-1u);
                    w_pixelLsb      = (w_tileData[2u*w_y] & (1u << (GPU_TILE_WIDTH-w_x-1u))) >> (GPU_TILE_WIDTH-w_x-1u);
                    w_pixelValue    = (w_pixelMsb << 1u) + w_pixelLsb;

                    // Conversion en couleur
                    switch(w_pixelValue)
                    {
                        case 0u:
                            w_pixelColor = GPU_WHITE;
                            break;

                        case 1u:
                            w_pixelColor = GPU_LIGHT_GRAY;
                            break;

                        case 2u:
                            w_pixelColor = GPU_DARK_GRAY;
                            break;

                        case 3u:
                            w_pixelColor = GPU_BLACK;
                            break;
                    }

                    // Sauvegarde du pixel dans le background
                    m_background[(GPU_TILE_WIDTH*w_i)+w_y][(GPU_TILE_HEIGHT*w_j)+w_x] = w_pixelColor;
                }
            }
        }
    }

    // Recopie dans l'image à afficher sur l'écran
    for (std::uint8_t w_j = 0; w_j < GAMEBOY_SCREEN_HEIGHT; ++w_j)
    {
        for (std::uint8_t w_i = 0; w_i < GAMEBOY_SCREEN_WIDTH; ++w_i)
        {
            ao_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = m_background[w_j][w_i];
        }
    }
}
