#include "Client.h"
#include "Window.h"
#include "Shader.h"

#include "SDL.h"


#ifdef WIN32
#	include <GL/glew.h>
#else
#	define GL3_PROTOTYPES 1
#	include <GL3/gl3.h>
#endif

int main(int argc, char **argv)
{
	Initialize();
	{
		Window window{ { 800, 600, false, "Test" } };

		SDL_Event evt;

		float vertices[] = { -0.5, 0.0, 0.5, -0.5, 0.5, 0.5 };
		float couleurs[] = { 0.0, 204.0 / 255.0, 1.0, 1.0, 204.0 / 255.0, 1.0, 0.0, 100.0 / 255.0, 1.0 };

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, couleurs);

		Shader shaderCouleur("Shaders/test2D.vert", "Shaders/test2D.frag");


		while (true)
		{
			SDL_WaitEvent(&evt);

			if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
				break;


			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderCouleur.GetId());

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);

			glUseProgram(0);

			

			window.Show();
		}		
	}
	Shutdown();
	return 0;
}