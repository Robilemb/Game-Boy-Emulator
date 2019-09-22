#include <thread>

#include "CORE/INCLUDE/gpu.h"

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

// Constructeur
Gpu::Gpu(Mpu* const aip_mpu, updateScreenFunction ai_updateScreen) :
    mp_mpu(aip_mpu),
    updateScreen(ai_updateScreen),
    m_fpsClock(std::chrono::high_resolution_clock::now()),
    m_mode(E_HBLANK),
    m_nbCylces(0u),
    m_curLine(0u)
{
    // Initialisation du background
    for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_WIDTH; ++w_i)
    {
        for (std::uint16_t w_j = 0u; w_j < GPU_BACKGROUND_HEIGHT; ++w_j)
        {
            m_background[w_i][w_j]  = 0u;
            m_window[w_i][w_j]      = 0u;
            m_sprites[w_i][w_j]     = 0u;
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

void Gpu::computeScreenImage(const std::uint8_t ai_cpuCycles)
{
    // Mise à jour du nombre de cycles du GPU
    m_nbCylces += ai_cpuCycles;

    // Valeur du registre IF
    std::uint8_t w_ifRegister = mp_mpu->getMemVal(GAMEBOY_INTERRUPT_FLAG);

    // Gestion du mode de fonctionnement
    switch (m_mode)
    {
        case E_HBLANK:
            if (m_nbCylces >= GPU_COMPUTE_LINE_NB_CYCLES)
            {
                // Reset du nombre de cycles
                m_nbCylces = 0u;

                // Incrémentation du numéro de ligne
                m_curLine++;

                // Mise à jour du registre LY
                mp_mpu->setMemVal(GPU_LY_ADDRESS, m_curLine);

                if (m_curLine == (GAMEBOY_SCREEN_HEIGHT - 1u))
                {
                    // Passage en VBLANK si l'ensemble des lignes ont été écrites
                    m_mode = E_VBLANK;

                    // Demande d'interruption VBLANK
                    mp_mpu->setMemVal(GAMEBOY_INTERRUPT_FLAG, (w_ifRegister | GAMEBOY_VBLANK_REQUESTED));
                }
            }

            break;

        case E_VBLANK:
            if (m_nbCylces >= GPU_COMPUTE_LINE_NB_CYCLES)
            {
                // Reset du nombre de cycles
                m_nbCylces = 0u;

                // Incrémentation du numéro de ligne
                m_curLine++;

                // Mise à jour du registre LY
                mp_mpu->setMemVal(GPU_LY_ADDRESS, m_curLine);

                if (m_curLine >= (GPU_VBLANK_NB_LINES + GAMEBOY_SCREEN_HEIGHT - 1u))
                {
                    // Calcul de l'image à afficher sur l'écran
                    _computeScreenImage();

                    // Mise à jour de l'écran
                    updateScreen(m_screenImage);

                    // Gestion du nombre d'images par seconde
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(GPU_FRAME_PERIOD_MS) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_fpsClock).count()));
                    m_fpsClock = std::chrono::high_resolution_clock::now();

                    // Reset de l'indice de ligne courante
                    m_curLine = 0u;

                    // Passage en mode HBLANK
                    m_mode = E_HBLANK;
                }
            }

            break;

        default:
            std::cout << "Erreur : Mode de fonctionnement du GPU inconnu" << std::endl;
    }
}


// ****************************************
// Fonctions privées
// ****************************************

void Gpu::_computeScreenImage()
{
    // Variables locales
    std::uint8_t    w_tileData[GPU_TILE_NB_BYTES];
    std::uint8_t    w_bgp[GPU_NB_COLORS];

    std::int16_t    w_tileNumber                    = 0;
    std::int8_t     w_tileNumberMode0               = 0;
    std::uint8_t    w_tileNumberMode1               = 0u;
    std::uint8_t    w_tileNumerOffset               = 0u;
    std::uint8_t    w_pixelLsb                      = 0u;
    std::uint8_t    w_pixelMsb                      = 0u;
    std::uint8_t    w_pixelValue                    = 0u;
    std::uint8_t    w_pixelColor                    = 0u;

    std::uint8_t    w_lcdDisplayEnable              = 0u;
    std::uint8_t    w_windowTileMapDisplaySelect    = 0u;
    std::uint8_t    w_windowDisplayEnable           = 0u;
    std::uint8_t    w_bgAndWindowTileDataSelect     = 0u;
    std::uint8_t    w_bgTileMapDisplaySelect        = 0u;
    //std::uint8_t    w_spriteSize                    = 0u;
    std::uint8_t    w_spriteDisplayEnable           = 0u;
    std::uint8_t    w_bgDisplay                     = 0u;

    std::uint16_t w_bgAndWindowTileDataOffset       = 0u;
    std::uint16_t w_bgTileMapDataOffset             = 0u;
    std::uint16_t w_windowTileMapDataOffset         = 0u;

    std::uint8_t w_scrollX                          = 0u;
    std::uint8_t w_scrollY                          = 0u;

    std::uint8_t w_windowX                          = 0u;
    std::uint8_t w_windowY                          = 0u;

    std::int16_t w_spriteX                          = 0u;
    std::int16_t w_spriteY                          = 0u;

    std::uint8_t w_bgLoadX                          = 0u;
    std::uint8_t w_bgLoadY                          = 0u;

    // Décodage du registre LDCD
    w_lcdDisplayEnable              = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 7u)   & 0x01;
    w_windowTileMapDisplaySelect    = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 6u)   & 0x01;
    w_windowDisplayEnable           = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 5u)   & 0x01;
    w_bgAndWindowTileDataSelect     = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 4u)   & 0x01;
    w_bgTileMapDisplaySelect        = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 3u)   & 0x01;
    //w_spriteSize                    = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 2u)   & 0x01;
    w_spriteDisplayEnable           = (mp_mpu->getMemVal(GPU_LCDC_ADDRESS) >> 1u)   & 0x01;
    w_bgDisplay                     = mp_mpu->getMemVal(GPU_LCDC_ADDRESS)           & 0x01;

    if (w_lcdDisplayEnable == 1u)
    {
        // Mise à jour de la palette
        for (std::uint8_t w_i = 0u; w_i < GPU_NB_COLORS; ++w_i)
        {
            // Récupération du niveau de gris de la couleur w_i
            switch ((mp_mpu->getMemVal(GPU_BGP_ADDRESS) >> (2u*w_i)) & 0x03)
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

                default:
                    std::cout << "Erreur : Couleur de pixel inconnue" << std::endl;
                    exit(-1);
            }

            // Sauvegarde dans la palette
            w_bgp[w_i] = w_pixelColor;
        }

        // Offset des données du background et de la window à utiliser
        if (w_bgAndWindowTileDataSelect == 0u)
        {
            w_bgAndWindowTileDataOffset = GPU_BG_WINDOW_TILE_DATA_ADDRESS_MODE_0;
            w_tileNumerOffset           = 128u;
        }
        else
        {
            w_bgAndWindowTileDataOffset = GPU_BG_WINDOW_TILE_DATA_ADDRESS_MODE_1;
            w_tileNumerOffset           = 0u;
        }

        if (w_bgTileMapDisplaySelect == 0u)
        {
            w_bgTileMapDataOffset = GPU_BG_WINDOW_TILE_MAP_DATA_ADDRESS_MODE_0;
        }
        else
        {
            w_bgTileMapDataOffset = GPU_BG_WINDOW_TILE_MAP_DATA_ADDRESS_MODE_1;
        }

        if (w_windowTileMapDisplaySelect == 0u)
        {
            w_windowTileMapDataOffset = GPU_BG_WINDOW_TILE_MAP_DATA_ADDRESS_MODE_0;
        }
        else
        {
            w_windowTileMapDataOffset = GPU_BG_WINDOW_TILE_MAP_DATA_ADDRESS_MODE_1;
        }

        // Offsets X/Y de l'image dans le background
        w_scrollX = mp_mpu->getMemVal(GPU_SCX_ADDRESS);
        w_scrollY = mp_mpu->getMemVal(GPU_SCY_ADDRESS);

        // Offsets X/Y de la window
        w_windowX = mp_mpu->getMemVal(GPU_WX_ADDRESS);
        w_windowY = mp_mpu->getMemVal(GPU_WY_ADDRESS);

        // Gestion des bornes de la window
        if (w_windowX < GPU_WINDOW_HORIZONTAL_OFFSET)
        {
            w_windowX = GPU_WINDOW_HORIZONTAL_OFFSET;
        }

        if (w_windowX > (GAMEBOY_SCREEN_WIDTH + GPU_WINDOW_HORIZONTAL_OFFSET - 1u))
        {
            w_windowX = (GAMEBOY_SCREEN_WIDTH + GPU_WINDOW_HORIZONTAL_OFFSET - 1u);
        }

        if (w_windowY > (GAMEBOY_SCREEN_HEIGHT - 1u))
        {
            w_windowY = (GAMEBOY_SCREEN_HEIGHT - 1u);
        }

        // Prise en compte de l'offset X de la window
        w_windowX -= GPU_WINDOW_HORIZONTAL_OFFSET;

        // Chargement des tiles du background
        for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_TILE_WIDTH; ++w_i)
        {
            for (std::uint8_t w_j = 0u; w_j < GPU_BACKGROUND_TILE_HEIGHT; ++w_j)
            {
                // Numéro de tile à charger
                if (w_bgAndWindowTileDataSelect == 0u)
                {
                    w_tileNumberMode0   = static_cast<int8_t>(mp_mpu->getMemVal(w_bgTileMapDataOffset + w_j + (w_i*GPU_BACKGROUND_TILE_HEIGHT)));
                    w_tileNumber        = static_cast<int16_t>(w_tileNumberMode0) + static_cast<int16_t>(w_tileNumerOffset);
                }
                else
                {
                    w_tileNumberMode1   = static_cast<uint8_t>(mp_mpu->getMemVal(w_bgTileMapDataOffset + w_j + (w_i*GPU_BACKGROUND_TILE_HEIGHT)));
                    w_tileNumber        = static_cast<int16_t>(w_tileNumberMode1) + static_cast<int16_t>(w_tileNumerOffset);
                }

                for (std::uint8_t w_k = 0u; w_k < GPU_TILE_NB_BYTES; ++w_k)
                {
                    // Récupération des octets de représentation de la tile
                    w_tileData[w_k] = mp_mpu->getMemVal(static_cast<uint16_t>(static_cast<int16_t>(w_bgAndWindowTileDataOffset) + (static_cast<int16_t>(GPU_TILE_NB_BYTES)*w_tileNumber) + static_cast<int16_t>(w_k)));
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

        // Chargement des tiles de la window
        for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_TILE_WIDTH; ++w_i)
        {
            for (std::uint8_t w_j = 0u; w_j < GPU_BACKGROUND_TILE_HEIGHT; ++w_j)
            {
                // Numéro de tile à charger
                if (w_bgAndWindowTileDataSelect == 0u)
                {
                    w_tileNumberMode0   = static_cast<int8_t>(mp_mpu->getMemVal(w_windowTileMapDataOffset + w_j + (w_i*GPU_BACKGROUND_TILE_HEIGHT)));
                    w_tileNumber        = static_cast<int16_t>(w_tileNumberMode0) + static_cast<int16_t>(w_tileNumerOffset);
                }
                else
                {
                    w_tileNumberMode1   = static_cast<uint8_t>(mp_mpu->getMemVal(w_windowTileMapDataOffset + w_j + (w_i*GPU_BACKGROUND_TILE_HEIGHT)));
                    w_tileNumber        = static_cast<int16_t>(w_tileNumberMode1) + static_cast<int16_t>(w_tileNumerOffset);
                }

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

                        // Sauvegarde du pixel dans la window
                        m_window[(GPU_TILE_WIDTH*w_i) + w_y][(GPU_TILE_HEIGHT*w_j) + w_x] = w_bgp[w_pixelValue];
                    }
                }
            }
        }

        // Reset des sprites
        for (std::uint16_t w_i = 0u; w_i < GPU_BACKGROUND_WIDTH; ++w_i)
        {
            for (std::uint16_t w_j = 0u; w_j < GPU_BACKGROUND_HEIGHT; ++w_j)
            {
                m_sprites[w_i][w_j] = w_bgp[0u];
            }
        }

        // Chargement des sprites
        for (std::uint8_t w_spriteNumber = 0u; w_spriteNumber < GPU_SPRITES_MAX_NB; ++w_spriteNumber)
        {
            w_tileNumber    = mp_mpu->getMemVal(GPU_OAM_START_ADDRESS + (w_spriteNumber*GPU_SPRITES_ATTRIBUTE_SIZE_BYTE) + 2u);
            w_spriteX       = mp_mpu->getMemVal(GPU_OAM_START_ADDRESS + (w_spriteNumber*GPU_SPRITES_ATTRIBUTE_SIZE_BYTE) + 1u) + w_scrollX - GPU_SPRITES_HORIZONTAL_OFFSET;
            w_spriteY       = mp_mpu->getMemVal(GPU_OAM_START_ADDRESS + (w_spriteNumber*GPU_SPRITES_ATTRIBUTE_SIZE_BYTE)     ) + w_scrollY - GPU_SPRITES_VERTICAL_OFFSET;

            for (std::uint8_t w_k = 0u; w_k < GPU_TILE_NB_BYTES; ++w_k)
            {
                // Récupération des octets de représentation du sprite
                w_tileData[w_k] = mp_mpu->getMemVal(GPU_SPRITES_DATA_START_ADDRESS + (GPU_TILE_NB_BYTES*w_tileNumber) + w_k);
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

                    // Sauvegarde de la sprite si le pixel est bien contenu dans le background
                    if (    (w_spriteX + static_cast<std::int16_t>(w_x) >= 0)
                        &&  (w_spriteX + static_cast<std::int16_t>(w_x) < static_cast<std::int16_t>(GPU_BACKGROUND_WIDTH))
                        &&  (w_spriteY + static_cast<std::int16_t>(w_y) >= 0)
                        &&  (w_spriteY + static_cast<std::int16_t>(w_y) < static_cast<std::int16_t>(GPU_BACKGROUND_HEIGHT)) )
                    {
                        m_sprites[w_spriteY + w_y][w_spriteX + w_x] = w_bgp[w_pixelValue];
                    }
                }
            }
        }

        // Sauvegarde dans l'image à afficher sur l'écran
        for (std::uint8_t w_j = 0; w_j < GAMEBOY_SCREEN_HEIGHT; ++w_j)
        {
            for (std::uint8_t w_i = 0; w_i < GAMEBOY_SCREEN_WIDTH; ++w_i)
            {
                // Priorité 1 : chargement des sprites
                if ( (w_spriteDisplayEnable == 1u) && (m_sprites[w_j+w_scrollY][w_i+w_scrollX] != w_bgp[0u]) )
                {
                    m_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = m_sprites[w_j+w_scrollY][w_i+w_scrollX];
                }
                // Priorité 2 : chargement de la window
                else if (   (w_windowDisplayEnable == 1u)
                         && (w_i >= w_windowX)
                         && (w_j >= w_windowY)
                         && ((static_cast<std::uint16_t>(w_i) + static_cast<std::uint16_t>(w_windowX)) < static_cast<std::uint16_t>(GPU_BACKGROUND_WIDTH))
                         && ((static_cast<std::uint16_t>(w_j) + static_cast<std::uint16_t>(w_windowY)) < static_cast<std::uint16_t>(GPU_BACKGROUND_HEIGHT)))
                {
                    m_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = m_window[w_j-w_windowY][w_i-w_windowX];
                }
                // Priorité 3 : chargement du background
                else if (w_bgDisplay == 1u)
                {
                    // Gestion des dépassements en X
                    if (static_cast<std::uint16_t>(w_i) + static_cast<std::uint16_t>(w_scrollX) >= static_cast<std::uint16_t>(GPU_BACKGROUND_WIDTH))
                    {
                        w_bgLoadX = static_cast<std::uint8_t>(static_cast<std::uint16_t>(w_i) + static_cast<std::uint16_t>(w_scrollX) - static_cast<std::uint16_t>(GPU_BACKGROUND_WIDTH));
                    }
                    else
                    {
                        w_bgLoadX = w_i + w_scrollX;
                    }

                    // Gestion des dépassements en Y
                    if (static_cast<std::uint16_t>(w_j) + static_cast<std::uint16_t>(w_scrollY) >= static_cast<std::uint16_t>(GPU_BACKGROUND_HEIGHT))
                    {
                        w_bgLoadY = static_cast<std::uint8_t>(static_cast<std::uint16_t>(w_j) + static_cast<std::uint16_t>(w_scrollY) - static_cast<std::uint16_t>(GPU_BACKGROUND_HEIGHT));
                    }
                    else
                    {
                        w_bgLoadY = w_j + w_scrollY;
                    }

                    m_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = m_background[w_bgLoadY][w_bgLoadX];
                }
                else
                {
                    // Background blanc si affichage OFF
                    m_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = GPU_WHITE;
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
                m_screenImage[w_i + (w_j*GAMEBOY_SCREEN_WIDTH)] = GPU_WHITE;
            }
        }
    }
}
