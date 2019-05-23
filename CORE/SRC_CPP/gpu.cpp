#include "CORE/INCLUDE/gpu.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gpu::Gpu(Mpu* const aip_mpu) :
    mp_mpu(aip_mpu)
{
    // Initialisation du background
    for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_WIDTH; ++w_i)
    {
        for (std::uint16_t w_j = 0u; w_j < GPU_BACKGROUND_HEIGHT; ++w_j)
        {
            m_background[w_i][w_j] = 0u;
        }
    }
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
    std::uint8_t    w_tileData[GPU_TILE_NB_BYTES];
    std::uint8_t    w_bgp[GPU_NB_COLORS];

    std::uint8_t    w_tileNumber                    = 0u;
    std::uint8_t    w_pixelLsb                      = 0u;
    std::uint8_t    w_pixelMsb                      = 0u;
    std::uint8_t    w_pixelValue                    = 0u;
    std::uint8_t    w_pixelColor                    = 0u;

    std::uint8_t    w_lcdDisplayEnable              = 0u;
    //std::uint8_t    w_windowTileMapDisplaySelect    = 0u;
    //std::uint8_t    w_windowDisplayEnable           = 0u;
    std::uint8_t    w_bgAndWindowTileDataSelect     = 0u;
    std::uint8_t    w_bgTileMapDisplaySelect        = 0u;
    //std::uint8_t    w_objSize                       = 0u;
    //std::uint8_t    w_objDisplayEnable              = 0u;
    std::uint8_t    w_bgDisplay                     = 0u;

    std::uint16_t w_bgAndWindowTileDataOffset       = 0u;
    std::uint16_t w_bgTileMapDataOffset             = 0u;

    std::uint8_t w_scrollX                          = 0u;
    std::uint8_t w_scrollY                          = 0u;

    // Décodage du registre LDCD
    w_lcdDisplayEnable              = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 7u)   & 0x01;
    //w_windowTileMapDisplaySelect    = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 6u)   & 0x01;
    //w_windowDisplayEnable           = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 5u)   & 0x01;
    w_bgAndWindowTileDataSelect     = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 4u)   & 0x01;
    w_bgTileMapDisplaySelect        = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 3u)   & 0x01;
    //w_objSize                       = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 2u)   & 0x01;
    //w_objDisplayEnable              = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 1u)   & 0x01;
    w_bgDisplay                     = mp_mpu->getMemVal(GPU_LCDC_ADDRESS)           & 0x01;

    // Mise à 0 du registre LY
    mp_mpu->setMemVal(GPU_LY_ADDRESS, 0u);

    if (w_lcdDisplayEnable == 1u)
    {
        // Mise à jour de la palette
        for (std::uint8_t w_i = 0u; w_i < GPU_NB_COLORS; ++w_i)
        {
            // Récupération du niveau de gris de la couleur w_i
            switch((mp_mpu->getMemVal(GPU_BGP_ADDRESS) >> (2u*w_i)) & 0x03)
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

            // Sauvegarde dans la palette
            w_bgp[w_i] = w_pixelColor;
        }

        // Offset des données du background à utiliser
        if (w_bgAndWindowTileDataSelect == 0u)
        {
            w_bgAndWindowTileDataOffset = GPU_BG_WINDOW_TILE_DATA_OFFSET_MODE_0;
        }
        else
        {
            w_bgAndWindowTileDataOffset = GPU_BG_WINDOW_TILE_DATA_OFFSET_MODE_1;
        }

        if (w_bgTileMapDisplaySelect == 0u)
        {
            w_bgTileMapDataOffset = GPU_BG_TILE_MAP_DATA_OFFSET_MODE_0;
        }
        else
        {
            w_bgTileMapDataOffset = GPU_BG_TILE_MAP_DATA_OFFSET_MODE_1;
        }

        // Offsets X/Y de l'image dans le background
        w_scrollX = mp_mpu->getMemVal(GPU_SCX_ADDRESS);
        w_scrollY = mp_mpu->getMemVal(GPU_SCY_ADDRESS);

        // Correction des offsets X si sortie du background
        if ((w_scrollX + GAMEBOY_SCREEN_WIDTH) > GPU_BACKGROUND_WIDTH)
        {
            w_scrollX = GPU_BACKGROUND_WIDTH - GAMEBOY_SCREEN_WIDTH;
        }

        // Correction des offsets Y si sortie du background
        if ((w_scrollY + GAMEBOY_SCREEN_HEIGHT) > GPU_BACKGROUND_HEIGHT)
        {
            w_scrollY = GPU_BACKGROUND_HEIGHT - GAMEBOY_SCREEN_HEIGHT;
        }

        // Chargement des tiles correspondantes dans m_background
        for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_TILE_WIDTH; ++w_i)
        {
            for (std::uint8_t w_j = 0u; w_j < GPU_BACKGROUND_TILE_HEIGHT; ++w_j)
            {
                // Numéro de tile à charger
                w_tileNumber = mp_mpu->getMemVal(w_bgTileMapDataOffset + w_j + (w_i*GPU_BACKGROUND_TILE_HEIGHT));

                for (std::uint8_t w_k = 0u; w_k < GPU_TILE_NB_BYTES; ++w_k)
                {
                    // Récupération des octets de représentation de la tile
                    w_tileData[w_k] = mp_mpu->getMemVal(w_bgAndWindowTileDataOffset + (GPU_TILE_NB_BYTES*w_tileNumber) + w_k);
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

                        // Sauvegarde du pixel dans le background
                        m_background[(GPU_TILE_WIDTH*w_i) + w_y][(GPU_TILE_HEIGHT*w_j) + w_x] = w_bgp[w_pixelValue];
                    }
                }
            }
        }

        // Sauvegarde dans l'image à afficher sur l'écran
        for (std::uint8_t w_j = 0; w_j < GAMEBOY_SCREEN_HEIGHT; ++w_j)
        {
            for (std::uint8_t w_i = 0; w_i < GAMEBOY_SCREEN_WIDTH; ++w_i)
            {
                if (w_bgDisplay == 1u)
                {
                    // Recopie du background
                    ao_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = m_background[w_j+w_scrollY][w_i+w_scrollX];
                }
                else
                {
                    // Background blanc si affichage OFF
                    ao_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = GPU_WHITE;
                }
            }
        }
    }
    else
    {
        // Image blanche si l'écran n'est pas allumé
        for (std::uint8_t w_j = 0; w_j < GAMEBOY_SCREEN_HEIGHT; ++w_j)
        {
            for (std::uint8_t w_i = 0; w_i < GAMEBOY_SCREEN_WIDTH; ++w_i)
            {
                ao_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = GPU_WHITE;
            }
        }
    }

    // Mise à GAMEBOY_SCREEN_HEIGHT du registre LY
    mp_mpu->setMemVal(GPU_LY_ADDRESS, GAMEBOY_SCREEN_HEIGHT);
}
