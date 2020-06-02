#include <Application.h>
#include <iostream>

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
    SDL_Renderer* renderer = SDL_CreateRenderer(
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

    pixel.h = PIXEL_SIZE;
    pixel.w = PIXEL_SIZE;
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
    }
}

void Application::update(double delta_time)
{
    // implement
}

void Application::draw()
{
    for(int y = 0; y < CHIP8_HEIGHT; y++)
    {
        for(int x = 0; x < CHIP8_WIDTH; x++)
        {
            // set rect attributes
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