#ifndef GPU_H
#define GPU_H

#include <array>

#include "shared_data.h"
#include "mpu.h"

#define GPU_BACKGROUND_WIDTH    256u
#define GPU_BACKGROUND_HEIGHT   256u
#define GPU_BACKGROUND_SIZE     (GPU_BACKGROUND_WIDTH*GPU_BACKGROUND_HEIGHT)

#define GPU_TILE_WIDTH          8u
#define GPU_TILE_HEIGHT         8u
#define GPU_TILE_SIZE           (GPU_TILE_WIDTH*GPU_TILE_HEIGHT)

#define GPU_WHITE               255u
#define GPU_LIGHT_GRAY          190u
#define GPU_DARK_GRAY           90u
#define GPU_BLACK               0u

class Gpu
{
public:
    explicit Gpu(Mpu* const aip_mpu);
    ~Gpu();

    void computeScreenImage(gbScreenImage& ai_screenImage);

private:
    Mpu*                                            mp_mpu;         // Pointeur vers la mémoire
    std::array<std::uint8_t, GPU_BACKGROUND_SIZE>   m_background;   // Background
};

#endif // GPU_H
