#include "UserInterface.h"

#include "SDL_ttf.h"

void InitializeUI()
{
	TTF_Init();
}

void ShutdownUI()
{
	TTF_Quit();
}