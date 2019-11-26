#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <functional>

// Taille de l'écran
#define GAMEBOY_SCREEN_WIDTH        160u
#define GAMEBOY_SCREEN_HEIGHT       144u
#define GAMEBOY_SCREEN_SIZE         (GAMEBOY_SCREEN_WIDTH*GAMEBOY_SCREEN_HEIGHT)

// Interruptions
#define GAMEBOY_VBLANK_ENABLE       0x01
#define GAMEBOY_VBLANK_REQUESTED    0x01
#define GAMEBOY_LCD_STAT_ENABLE     0x02
#define GAMEBOY_LCD_STAT_REQUESTED  0x02
#define GAMEBOY_TIMER_ENABLE        0x04
#define GAMEBOY_TIMER_REQUESTED     0x04
#define GAMEBOY_SERIAL_ENABLE       0x08
#define GAMEBOY_SERIAL_REQUESTED    0x08
#define GAMEBOY_JOYPAD_ENABLE       0x10
#define GAMEBOY_JOYPAD_REQUESTED    0x10

typedef std::array<std::uint8_t, GAMEBOY_SCREEN_SIZE> gbScreenImage;

typedef std::function<void(const gbScreenImage&)> updateScreenFunction;

#endif // SHARED_DATA_H
