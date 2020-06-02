#pragma once

#include <SDL2/SDL.h>
#include "Chip8.h"

#define WINDOW_NAME   "CHIP 8 EMULATOR"
#define CHIP8_WIDTH   64
#define CHIP8_HEIGHT  32
#define PIXEL_SIZE    10
#define WINDOW_WIDTH  CHIP8_WIDTH  * PIXEL_SIZE
#define WINDOW_HEIGHT CHIP8_HEIGHT * PIXEL_SIZE

class Application
{
public:
    Application(const char* file);
    ~Application();
    void loop();
private:
    void update();
    void draw();
    SDL_Rect pixel;
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Event event;
    Chip8 chip8;
};