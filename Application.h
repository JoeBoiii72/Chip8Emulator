#pragma once

#include <SDL2/SDL.h>

#define WINDOW_NAME   "CHIP 8 EMULATOR"
#define CHIP8_WIDTH   64
#define CHIP8_HEIGHT  32
#define PIXEL_SIZE    10
#define WINDOW_WIDTH  CHIP8_WIDTH  * PIXEL_SIZE
#define WINDOW_HEIGHT CHIP8_HEIGHT * PIXEL_SIZE
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

class Application
{
public:
    Application();
    ~Application();
    void loop();
private:
    void update(double delta_time);
    void draw();
    bool bitmap[CHIP8_HEIGHT*CHIP8_WIDTH];
    SDL_Rect pixel;
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Event event;
};