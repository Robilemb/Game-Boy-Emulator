#ifndef GPU_H
#define GPU_H

#include <array>
#include <chrono>

#include "shared_data.h"
#include "mpu.h"

// Gestion du background et des tiles
#define GPU_BACKGROUND_WIDTH                    256u
#define GPU_BACKGROUND_HEIGHT                   256u
#define GPU_BACKGROUND_SIZE                     (GPU_BACKGROUND_WIDTH*GPU_BACKGROUND_HEIGHT)

#define GPU_BACKGROUND_TILE_WIDTH               32u
#define GPU_BACKGROUND_TILE_HEIGHT              32u
#define GPU_BACKGROUND_TILE_SIZE                (GPU_BACKGROUND_TILE_WIDTH*GPU_BACKGROUND_TILE_HEIGHT)

#define GPU_TILE_WIDTH                          8u
#define GPU_TILE_HEIGHT                         8u
#define GPU_TILE_SIZE                           (GPU_TILE_WIDTH*GPU_TILE_HEIGHT)

#define GPU_TILE_NB_BYTES                       16u

#define GPU_BG_WINDOW_TILE_DATA_ADDRESS_MODE_0  0x8800
#define GPU_BG_WINDOW_TILE_DATA_ADDRESS_MODE_1  0x8000

#define GPU_BG_TILE_MAP_DATA_ADDRESS_MODE_0     0x9800
#define GPU_BG_TILE_MAP_DATA_ADDRESS_MODE_1     0x9C00

// Gestion des sprites
#define GPU_SPRITES_MAX_NB                      40u

#define GPU_OAM_START_ADDRESS                   0xFE00
#define GPU_SPRITES_ATTRIBUTE_SIZE_BYTE         4u

#define GPU_SPRITES_HORIZONTAL_OFFSET           8u
#define GPU_SPRITES_VERTICAL_OFFSET             16u

#define GPU_SPRITES_DATA_START_ADDRESS          0x8000

// Niveaux de gris d'affichage
#define GPU_NB_COLORS                           4u

#define GPU_WHITE                               255u
#define GPU_LIGHT_GRAY                          170u
#define GPU_DARK_GRAY                           85u
#define GPU_BLACK                               0u

// Adresse des registres GPU
#define GPU_LCDC_ADDRESS                        0xFF40
#define GPU_SCY_ADDRESS                         0xFF42
#define GPU_SCX_ADDRESS                         0xFF43
#define GPU_LY_ADDRESS                          0xFF44
#define GPU_BGP_ADDRESS                         0xFF47

// Nombre de cycles de différents modes de fonctionnement
#define GPU_OAM_SEARCH_NB_CYCLES                20u
#define GPU_PIXEL_TRANSFER_NB_CYCLES            43u
#define GPU_HBLANK_NB_CYCLES                    51u

#define GPU_VBLANK_NB_LINES                     10u
#define GPU_COMPUTE_LINE_NB_CYCLES              (GPU_OAM_SEARCH_NB_CYCLES + GPU_PIXEL_TRANSFER_NB_CYCLES + GPU_HBLANK_NB_CYCLES)

// Nombre d'images par seconde
#define GPU_FRAME_PER_SECONDS                   59.7f
#define GPU_FRAME_PERIOD_MS                     (1000.0f/GPU_FRAME_PER_SECONDS)

class Gpu
{
public:
    explicit Gpu(Mpu* const aip_mpu, updateScreenFunction ai_updateScreen);
    ~Gpu();

    void computeScreenImage(const std::uint8_t ai_cpuCycles);

private:
    void _computeScreenImage();

private:
    // Modes de fonctionnement du GPU
    typedef enum
    {
        E_OAM_SEARCH        = 0u,
        E_PIXEL_TRANSFER,
        E_HBLANK,
        E_VBLANK
    } eModes;

private:
    Mpu*                                            mp_mpu;                                                     // Pointeur vers la mémoire

    gbScreenImage                                   m_screenImage;                                              // Image à afficher sur l'écran
    updateScreenFunction                            updateScreen;                                               // Callback de mise à jour de l'écran

    std::chrono::high_resolution_clock::time_point  m_fpsClock;                                                 // Horloge de mesure du nombre d'images par seconde

    eModes                                          m_mode;                                                     // Mode courant d'utilisation du GPU
    std::uint16_t                                   m_nbCylces;                                                 // Nombre de cylces d'exécution du GPU
    std::uint8_t                                    m_curLine;                                                  // Numéro de la dernière ligne écrite

    std::uint8_t                                    m_background[GPU_BACKGROUND_WIDTH][GPU_BACKGROUND_HEIGHT];  // Background
};

#endif // GPU_H
