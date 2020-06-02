#include "Application.h"
#include "Chip8.h"

int main( int argc, char* args[] )
{
	Application app;
	app.loop();
	SDL_Quit();	
	return 0;
}