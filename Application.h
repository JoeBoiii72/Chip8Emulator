#pragma once

#include <SDL2/SDL.h>
#include "Chip8.h"

#define WINDOW_NAME   "CHIP 8 EMULATOR"
#define CHIP8_WIDTH   64
#define CHIP8_HEIGHT  32
#define len(a) (sizeof(a)/sizeof(a[0])) 

class Application
{
public:
    Application(const char* file, int pixelSize);
    ~Application();
    void loop();
private:
    uint8_t controls[16];
    void update();
    void draw();
    void handleEvent();
    int pixelSize;
    SDL_Rect pixel;
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Event event;
    Chip8 chip8;
};