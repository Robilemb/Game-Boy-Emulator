#ifndef GPU_H
#define GPU_H

#include <array>

#include "shared_data.h"
#include "mpu.h"

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

#define GPU_BG_WINDOW_TILE_DATA_OFFSET_MODE_0   0x8800
#define GPU_BG_WINDOW_TILE_DATA_OFFSET_MODE_1   0x8000

#define GPU_BG_TILE_MAP_DATA_OFFSET_MODE_0      0x9800
#define GPU_BG_TILE_MAP_DATA_OFFSET_MODE_1      0x9C00

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

class Gpu
{
public:
    explicit Gpu(Mpu* const aip_mpu);
    ~Gpu();

    void computeScreenImage(gbScreenImage& ao_screenImage);

private:
    Mpu*            mp_mpu;                                                     // Pointeur vers la mémoire
    std::uint8_t    m_background[GPU_BACKGROUND_WIDTH][GPU_BACKGROUND_HEIGHT];  // Background
};

#endif // GPU_H
