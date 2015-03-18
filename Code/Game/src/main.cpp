#include "Client.h"
#include "Window.h"
#include "SDL.h"

int main(int argc, char **argv)
{
	Initialize();
	{
		Window window{ { 800, 600, false, "Test" } };

		SDL_Event evt;

		while (true)
		{
			SDL_WaitEvent(&evt);

			if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
				break;

		}

		
	}
	Shutdown();
	return 0;





	// Boucle principale



	

	return 0;

}