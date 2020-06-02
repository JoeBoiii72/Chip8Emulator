#include "Application.h"
#include <unistd.h>

Application::Application(const char* file)
{         
    // lets hope SDL is intialized correctly.
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
	{
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }   

    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );

    // if window couldnt be created, get outta there
    if(window == NULL)
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }    	

    // here where we will draw/render the pixels
    renderer = SDL_CreateRenderer(window, -1, 0);

    // pixel to draw on screen
	pixel.w = PIXEL_SIZE;
	pixel.h = PIXEL_SIZE;

    // create chip8 instance
	chip8 = Chip8();

    // try and load rom
    if(!chip8.loadROM(file))
	{
		printf( "Couldn't load the ROM, try another one!\n");
		exit(1);
	}

    // setup controls to be used
    controls[0]  = SDLK_x;
    controls[1]  = SDLK_1;
    controls[2]  = SDLK_2;
    controls[3]  = SDLK_3;
    controls[4]  = SDLK_q;
    controls[5]  = SDLK_w;
    controls[6]  = SDLK_e;
    controls[7]  = SDLK_a;
    controls[8]  = SDLK_s;
    controls[9]  = SDLK_d;
    controls[10] = SDLK_z;
    controls[11] = SDLK_c;
    controls[12] = SDLK_4;
    controls[13] = SDLK_r;
    controls[14] = SDLK_f;
    controls[15] = SDLK_v;
}

Application::~Application()
{
    SDL_DestroyWindow(window);
}


void Application::loop()
{
	// draw back ground!
	while(true)
	{
		update();

		while (SDL_PollEvent(&event))
			handleEvent();

		// if flag set draw shit
		if(chip8.drawFlag)
			draw();

        SDL_RenderPresent(renderer);

        // this is linux only this shit
        usleep(1200);
	}
}

void Application::handleEvent()
{
    char keyPressed = event.key.keysym.sym;

    switch(event.type)
    {
        case SDL_QUIT:
            exit(1);

        // no break statement
        // i want it to drop down to the for loop
        case SDL_KEYDOWN:
        {
            switch(keyPressed)
            {
                // key presses go here
                case SDLK_ESCAPE:
                    exit(1);
            }
        }

        case SDL_KEYUP:
        {
            for (int i = 0; i < len(controls); ++i)
            {
                if (keyPressed == controls[i])
                {
                    chip8.key[i] = (event.type == SDL_KEYDOWN) ? 1 : 0;	
                }	
            }
        }
    }
}

// only thing to do here is emulate a cycle
void Application::update()
{
    chip8.emulateCylce();
}

// draw chip8 frame buffer using SDL2
void Application::draw()
{
    SDL_RenderClear(renderer);
    chip8.drawFlag = false;
    for(int i = 0; i < len(chip8.frameBuffer); ++i)
    {
        int x = i % CHIP8_WIDTH;
        int y = i / CHIP8_WIDTH;

        pixel.x = x * PIXEL_SIZE;
        pixel.y = y * PIXEL_SIZE;

        // if set
        if(chip8.frameBuffer[i] != 0)
        {
            // draw filled black square
            SDL_SetRenderDrawColor(renderer, 0 , 0 , 0, 0);
            SDL_RenderFillRect(renderer, &pixel);	
        }
        else
        {
            // draw filled white square
            SDL_SetRenderDrawColor(renderer, 255 , 0 , 255, 0);
            SDL_RenderFillRect(renderer, &pixel);

            // draw black outline
            SDL_SetRenderDrawColor(renderer, 0 , 0 , 0, 0);
            SDL_RenderDrawRect(renderer, &pixel);
        }                
    }
}