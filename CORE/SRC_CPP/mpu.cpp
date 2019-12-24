#include "CORE/INCLUDE/mpu.h"

#define BOOTSTRAP   {   0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0x0e, \
                        0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3, 0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, \
                        0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b, \
                        0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22, 0x23, 0x05, 0x20, 0xf9, \
                        0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99, 0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20, \
                        0xf9, 0x2e, 0x0f, 0x18, 0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04, \
                        0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20, 0xf7, 0x1d, 0x20, 0xf2, \
                        0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62, 0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06, \
                        0x7b, 0xe2, 0x0c, 0x3e, 0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20, \
                        0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17, \
                        0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9, 0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, \
                        0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e, \
                        0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, \
                        0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e, 0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, \
                        0x21, 0x04, 0x01, 0x11, 0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20, \
                        0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x01, 0xe0, 0x50  }

// ********************************************************
// Constructeur / Destructeur
// ********************************************************

Mpu::Mpu()
{
    // Initialisation de la mémoire
    initMemory();

    // Initialisation des boutons
    m_directionButtons.joypadP14    = 0xF;
    m_utilityButtons.joypadP15      = 0xF;
    m_isDirectionButtonsSelected    = false;
    m_isUtilityButtonsSelected      = false;
}

Mpu::~Mpu()
{
}


// ********************************************************
// CHARGEMENT D'UNE ROM
// ********************************************************

void Mpu::setROMData(const std::uint16_t ai_offset, const std::uint8_t ai_val)
{
    m_memory[ai_offset] = ai_val;
}


// ********************************************************
// ACCESSEURS
// ********************************************************

std::uint8_t Mpu::getMemVal(const std::uint16_t ai_offset) const
{
    std::uint8_t w_memValue = 0u;

    switch (ai_offset)
    {
        case MPU_JOYPAD_ADDRESS :
            if (m_isDirectionButtonsSelected == true)
            {
                w_memValue = m_directionButtons.joypadP14;
                w_memValue |= 0x10;
            }
            else w_memValue &= 0xEF;

            if (m_isUtilityButtonsSelected == true)
            {
                w_memValue = m_utilityButtons.joypadP15;
                w_memValue |= 0x20;
            }
            else w_memValue &= 0xDF;

            break;

        default :
            w_memValue = m_memory[ai_offset];
    }

    return w_memValue;
}

void Mpu::setMemVal(const std::uint16_t ai_offset, const std::uint8_t ai_val)
{
    if (!(ai_offset <= 0x7FFF))
    {
        switch (ai_offset)
        {
            case MPU_JOYPAD_ADDRESS :
                m_isDirectionButtonsSelected = (((ai_val & 0x10) >> 4u) == 0u);
                m_isUtilityButtonsSelected = (((ai_val & 0x20) >> 4u) == 0u);
                break;

            case MPU_DIV_ADDRESS :
                m_memory[ai_offset] = 0u;
                break;

            case MPU_IF_ADDRESS :
                m_memory[ai_offset] = ai_val | 0xE0;
                break;

            case MPU_DMA_ADDRESS :
                _dma_transfert(ai_val);

            default :
                m_memory[ai_offset] = ai_val;
        }
    }
}


// ********************************************************
// INITIALISATION DE LA MEMOIRE
// ********************************************************

void Mpu::initMemory()
{
    // Variables locales
    std::uint8_t w_bootstrap[MPU_BOOTSTRAP_SIZE] = BOOTSTRAP;

    // Initialisation de la mémoire
    for (std::uint32_t w_i = 0u; w_i < MPU_MEMORY_SIZE; ++w_i)
    {
        m_memory[w_i] = 0u;
    }

    // Chargement du bootstrap
    for (std::uint16_t w_i = 0u; w_i < MPU_BOOTSTRAP_SIZE; ++w_i)
    {
        m_memory[w_i] = w_bootstrap[w_i];
    }

    // Registre Joypad
    m_memory[MPU_JOYPAD_ADDRESS] = 0x3F;

    // Registre IF
    m_memory[MPU_IF_ADDRESS] = 0xE0;
}


// ********************************************************
// GESTION DU JOYPAD
// ********************************************************

void Mpu::setJoypadUp(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_directionButtons.sButtons.up = 0u;
    else                        m_directionButtons.sButtons.up = 1u;
}

void Mpu::setJoypadDown(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_directionButtons.sButtons.down = 0u;
    else                        m_directionButtons.sButtons.down = 1u;
}

void Mpu::setJoypadLeft(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_directionButtons.sButtons.left = 0u;
    else                        m_directionButtons.sButtons.left = 1u;
}

void Mpu::setJoypadRight(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_directionButtons.sButtons.right = 0u;
    else                        m_directionButtons.sButtons.right = 1u;
}

void Mpu::setJoypadA(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_utilityButtons.sButtons.a = 0u;
    else                        m_utilityButtons.sButtons.a = 1u;
}

void Mpu::setJoypadB(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_utilityButtons.sButtons.b = 0u;
    else                        m_utilityButtons.sButtons.b = 1u;
}

void Mpu::setJoypadStart(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_utilityButtons.sButtons.start = 0u;
    else                        m_utilityButtons.sButtons.start = 1u;
}

void Mpu::setJoypadSelect(const bool ai_isPressed)
{
    if (ai_isPressed == true)   m_utilityButtons.sButtons.select = 0u;
    else                        m_utilityButtons.sButtons.select = 1u;
}

/*void Mpu::setJoypad(const std::uint8_t ai_joypad)
{
    void Gameboy::_setButtons()
    {
        // Récupération du type de boutons
        std::uint8_t w_buttonType = (mp_mpu->getMemVal(MPU_JOYPAD_ADDRESS) >> 4u) & 0x03;

        // Mise à jour du registre Joypad
        switch (w_buttonType)
        {
            case 0u :
            case 1u :
                // Touches directionnelles
                mp_mpu->setJoypad(m_utilityButtons.joypadP15);
                break;

            case 2u :
                // Touches utilitaires
                mp_mpu->setJoypad(m_directionButtons.joypadP14);
                break;
        }
    }

    m_memory[MPU_JOYPAD_ADDRESS] = (m_memory[MPU_JOYPAD_ADDRESS] & 0x30) | (ai_joypad & 0x0F);
}*/


// ********************************************************
// REGISTRE DIV
// ********************************************************

void Mpu::setDivider(const std::uint8_t ai_divider)
{
    m_memory[MPU_DIV_ADDRESS] = ai_divider;
}


// ********************************************************
// TRANSFERT DMA
// ********************************************************

void Mpu::_dma_transfert(const std::uint8_t ai_startAddress)
{
    // Adresse de transfert de départ
    std::uint16_t w_startAddress = static_cast<std::uint16_t>(ai_startAddress) * 0x100;

    // Copie des données vers l'OAM
    for (std::uint8_t w_i = 0u; w_i < MPU_OAM_SIZE; ++w_i)
        m_memory[MPU_OAM_START_ADDRESS + w_i] = m_memory[w_startAddress + w_i];
}
