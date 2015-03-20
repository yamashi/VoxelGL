#include "Client.h"
#include "Renderer.h"
#include "Log.h"
#include "Memory.h"
#include "TaskManager.h"
#include "UserInterface.h"
#include "ThreadLocal.h"

#include "UIFont.h"
#include "UIText.h"

#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "Cube.h"

void Initialize()
{
	InitializableMemory();
	InitializableTLS();
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
	ShutdownTLS();
	ShutdownMemory();
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

struct Camera
{
	float x{ 0.0f }, y{ 0.0f };
};

void GameLoop()
{
	Window window{ { 800, 600, false, "Test" } };

	SDL_Event evt;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	Shader shaderCouleur;
	shaderCouleur.Load("Shaders/test2D.vert", "Shaders/test2D.frag");

	glm::mat4 projection = glm::perspective(70.0, (double)800 / 600, 1.0, 10000.0);
	glm::mat4 uiProjection = glm::ortho(0.0f, (float)800.f, (float)600.f, 0.0f, 0.0f, 100.0f);
	glm::mat4 modelview;

	UIFont bebasFont("Fonts/bebas.ttf", 40);

	UIText txt(bebasFont);
	txt.SetText("test toast toaster ------        ");

	Cube c(1.0f);
	Camera camera;

	glm::vec3 position(0.0, 0.0, -2.0);

	while (true)
	{
		modelview = glm::mat4(1.0);

		float phiRadian = camera.y * M_PI / 180;
		float thetaRadian = camera.x * M_PI / 180;

		glm::vec3 orientation;

		orientation.x = cos(phiRadian) * sin(thetaRadian);
		orientation.y = sin(phiRadian);
		orientation.z = cos(phiRadian) * cos(thetaRadian);

		TaskManager::GetInstance().Run(10);

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
				else if (evt.key.keysym.sym == SDLK_ESCAPE)
				{
					return;
				}
				else if (evt.key.keysym.sym == SDLK_w)
				{
					position += orientation;
				}
				else if (evt.key.keysym.sym == SDLK_s)
				{
					position -= orientation;
				}
				else if (evt.key.keysym.sym == SDLK_d)
				{
					position += glm::normalize(glm::cross(orientation, glm::vec3(0.0, 1.0, 0.0)));
				}
				else if (evt.key.keysym.sym == SDLK_a)
				{
					position -= glm::normalize(glm::cross(orientation, glm::vec3(0.0, 1.0, 0.0)));
				}
				break;
			case SDL_MOUSEMOTION:
				//txt.SetPosition(txt.GetPositionX() + evt.motion.xrel, txt.GetPositionY() + evt.motion.yrel);
				camera.x -= evt.motion.xrel / 10.f;
				camera.y -= evt.motion.yrel / 10.f;
				break;
			}
		}

		
		modelview = glm::lookAt(position, position + orientation, glm::vec3(0.0, 1.0, 0.0));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		c.Draw(projection, modelview);
		{
			
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			txt.Draw(800.f, 600.f, uiProjection, modelview);
		}
		window.Show();
	}
}