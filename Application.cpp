#include "Application.h"
#include <iostream>

#define delay SDL_Delay
#define len(a) (sizeof(a)/sizeof(a[0])) 

Application::Application()
{
    // create window
    window = SDL_CreateWindow(
        WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if(!window)
    {
        std::cout << "Failed to create window\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }

    // create renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if(!renderer)
    {
        std::cout << "Failed to get renderer\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }
 
    // clear bitmap just in case
    for(int i = 0; i < len(bitmap); i++)
        bitmap[i] = false;
                	
}

Application::~Application()
{
    SDL_DestroyWindow(window);
}


void Application::loop()
{
    bool running = true;
    while(running)
    {
        while(SDL_PollEvent(&event) > 0)
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        update(1.0/60.0);
        draw();
        delay(50);
    }
}

void Application::update(double delta_time)
{
    // implement
}

void Application::draw()
{
    // draw background first
    pixel.x = 0;
    pixel.y = 0;
    pixel.w = WINDOW_WIDTH;
    pixel.h = WINDOW_HEIGHT;

    // set background colour
    SDL_SetRenderDrawColor(renderer, 255 , 255 , 0, 0);
    SDL_RenderFillRect(renderer, &pixel);

    // pixel should be pixel size
    pixel.w = PIXEL_SIZE;
    pixel.h = PIXEL_SIZE;
    for(int y = 0; y < CHIP8_HEIGHT; y++)
    {
        for(int x = 0; x < CHIP8_WIDTH; x++)
        {
            // set rect attributes
            //SDL_Rect pixel;
            pixel.x = x*PIXEL_SIZE;
            pixel.y = y*PIXEL_SIZE;

            // set to black
            SDL_SetRenderDrawColor(renderer, 0 , 0 , 0, 0);

            // if bit set, fill, else draw
            if(bitmap[x + CHIP8_WIDTH * y])
                SDL_RenderFillRect(renderer, &pixel);	
            else
                SDL_RenderDrawRect(renderer, &pixel);				
        }
    }
    SDL_RenderPresent(renderer);
}