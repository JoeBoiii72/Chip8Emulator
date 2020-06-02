#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdint-gcc.h>


#define WINDOW_NAME   "CHIP 8 EMULATOR"
#define CHIP8_WIDTH   64
#define CHIP8_HEIGHT  32
#define PIXEL_SIZE    10
#define WINDOW_WIDTH  CHIP8_WIDTH  * PIXEL_SIZE
#define WINDOW_HEIGHT CHIP8_HEIGHT * PIXEL_SIZE
#define TRUE          1
#define FALSE         0

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


const int image[] = 
{
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,0,
	1,0,0,0,0,
	1,1,1,1,1
};

int main( int argc, char* args[] )
{

	// The window we'll be rendering to
	SDL_Window* window = NULL;
	
	// The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		//Destroy window
		SDL_DestroyWindow( window );

		//Quit SDL subsystems
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// rectangle for pixel
	

	// pixel colours
	// bitmap
	bool bitmap[CHIP8_HEIGHT*CHIP8_WIDTH];

	//Create window
	window = SDL_CreateWindow( "SDL Tutorial",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );

	SDL_Rect rect;
	rect.w = PIXEL_SIZE;
	rect.h = PIXEL_SIZE;


	for(int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 5; x++)
		{
			int index_image  = x  + 5 * y;
			int index_bitmap = (x+10) + CHIP8_WIDTH * (y+10);
			bitmap[index_bitmap] = image[index_image];
		}
	}
	

	if(window)
	{
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		
		// set to white
		SDL_SetRenderDrawColor(renderer, 255 , 255 , 255, 0);

		// rect that fills screen
		rect.x = 0;
		rect.y = 0;
		rect.w = WINDOW_WIDTH;
		rect.h = WINDOW_HEIGHT;

		// fill screen
		SDL_RenderFillRect(renderer, &rect);

		SDL_Event m_window_event;
		bool running = true;
		while(m_window_event.type != SDL_QUIT)
		{
			while(SDL_PollEvent(&m_window_event))
			{
				if (m_window_event.type == SDL_QUIT)
				{
					SDL_Quit();
					exit(1);
				}
			}
			
			for(int y = 0; y < CHIP8_HEIGHT; y++)
			{
				for(int x = 0; x < CHIP8_WIDTH; x++)
				{
					// set rect attributes
					rect.x = x*PIXEL_SIZE;
					rect.y = y*PIXEL_SIZE;
					rect.w = PIXEL_SIZE;
					rect.h = PIXEL_SIZE;

					// set to black
					SDL_SetRenderDrawColor(renderer, 0 , 0 , 0, 0);

					// if bit set, fill, else draw
					if(bitmap[x + CHIP8_WIDTH * y])
						SDL_RenderFillRect(renderer, &rect);	
					else
						SDL_RenderDrawRect(renderer, &rect);				
				}
			}

			SDL_RenderPresent(renderer);
			SDL_Delay(100);
		}
	
	}
	else
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
	}

	SDL_Quit();	
	return 0;
}