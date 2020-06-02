#include "Application.h"

int main( int argc, char* args[] )
{
	Application app;
	app.loop();
	SDL_Quit();	
	return 0;
}