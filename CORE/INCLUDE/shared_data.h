#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <functional>

#define GAMEBOY_SCREEN_WIDTH    160u
#define GAMEBOY_SCREEN_HEIGHT   144u
#define GAMEBOY_SCREEN_SIZE     (GAMEBOY_SCREEN_WIDTH*GAMEBOY_SCREEN_HEIGHT)

typedef std::array<std::uint8_t, GAMEBOY_SCREEN_SIZE> gbScreenImage;

typedef std::function<void(const gbScreenImage&)> updateScreenFunction;

#endif // SHARED_DATA_H
