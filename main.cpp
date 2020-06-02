#include "Application.h"
#include <SDL2/SDL.h>

// sims 2 music

int main( int argc, char* args[] )
{
	if (argc != 2)
	{
       	printf("Yo, you need to input a ROM file\n");
        return 1;
    }

	Application app(args[1]);
	app.loop();
	return 0;
}