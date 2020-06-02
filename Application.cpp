#include "Application.h"
#include <unistd.h>

#define delay SDL_Delay
#define len(a) (sizeof(a)/sizeof(a[0])) 

const uint8_t controls[16] =
{
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

Application::Application(const char* file)
{         
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
	{
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }   

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );

	 if(window == NULL)
	 {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }    	

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Event event;

	SDL_Rect pixel;
	pixel.w = PIXEL_SIZE;
	pixel.h = PIXEL_SIZE;

	Chip8 chip8 = Chip8();

    if(!chip8.loadROM(file))
	{
		printf( "Couldn't load the ROM, try another one!\n");
		exit(1);
	}
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
				case SDL_KEYUP  :
				{
					for (int i = 0; i < 16; ++i)
					{
						if (keyPressed == controls[i])
						{
							chip8.key[i] = (event.type == SDL_KEYDOWN) ? 1 : 0;	
						}	
					}
				}
				break;
			}
        }

		// if flag set draw shit
		if(chip8.drawFlag)
			draw();

        SDL_RenderPresent(renderer);

        // this is linux only this shit
        usleep(1200);
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
    for(int i = 0; i < CHIP8_HEIGHT*CHIP8_WIDTH; ++i)
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