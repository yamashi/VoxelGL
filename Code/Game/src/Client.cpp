#include "Client.h"
#include "Renderer.h"
#include "Log.h"
#include "Memory.h"
#include "TaskManager.h"
#include "UserInterface.h"

#include "UIFont.h"
#include "UIText.h"

#include "Window.h"
#include "Renderer.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef WIN32
#	include <GL/glew.h>
#elif __APPLE__
#	define GL3_PROTOTYPES 1
#	include <OpenGL/gl3.h>
#else
#	define GL3_PROTOTYPES 1
#	include <GL3/gl3.h>
#endif

void Initialize()
{
	InitializeLog();
	TaskManager::Initialize();
	InitializeRenderer();
	InitializeUI();
}

void Shutdown()
{
	ShutdownUI();
	ShutdownRenderer();
	TaskManager::Shutdown();
	ShutdownLog();
}

class Test : public Task
{
public:

	int m_i;

	Test(int i){ m_i = i; }
	~Test(){}

	Task::State DoBackgroundWork()
	{
		m_i *= 2;
		return KNeedMainthreadTime;
	}

	Task::State DoMainthreadWork()
	{
		std::cout << "exec " << m_i << std::endl;
		//std::this_thread::sleep_for(std::chrono::duration<uint32_t, std::milli>(2));
		return KFinished;
	}
};

void GameLoop()
{
	Window window{ { 800, 600, false, "Test" } };

	SDL_Event evt;

	float vertices[] = { 1, -1, 1, 1, -1, 1,
						  1, -1, -1, -1, -1, 1 };
	float couleurs[] = { 0.0, 204.0 / 255.0, 1.0, 1.0, 204.0 / 255.0, 1.0, 0.0, 100.0 / 255.0, 1.0, 
						 0.0, 204.0 / 255.0, 1.0, 1.0, 204.0 / 255.0, 1.0, 0.0, 100.0 / 255.0, 1.0 };

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, couleurs);

	Shader shaderCouleur("Shaders/test2D.vert", "Shaders/test2D.frag");

	glm::mat4 projection;
	glm::mat4 modelview;

	projection = glm::perspective(70.0, (double)800 / 600, 1.0, 100.0);
	modelview = glm::mat4(1.0);
	modelview = glm::translate(modelview, glm::vec3(100.0, -2.0, 0.0));

	UIFont bebasFont("Fonts/bebas.ttf", 40);

	UIText txt(bebasFont);
	
	int ctr = 0;
	while (true)
	{
		++ctr;
		TaskManager::GetInstance().Run(10);

		txt.SetText("Test lol " + std::to_string(ctr));

		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_WINDOWEVENT:
				if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
					return;
				break;
			case SDL_KEYDOWN:
				if (evt.key.keysym.sym == SDLK_SPACE)
				{
					for (int i = 0; i < 1000; ++i)
						TaskManager::GetInstance().AddBackground(new Test(i));
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		/*glUseProgram(shaderCouleur.GetId());

		//glUniformMatrix4fv(glGetUniformLocation(shaderCouleur.GetId(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
		//glUniformMatrix4fv(glGetUniformLocation(shaderCouleur.GetId(), "projection"), 1, GL_FALSE, value_ptr(projection));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glUseProgram(0);*/

		txt.Draw();

		window.Show();
	}
}