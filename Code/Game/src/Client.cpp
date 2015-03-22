#include "Client.h"
#include "Renderer.h"
#include "Log.h"
#include "Memory.h"
#include "TaskManager.h"
#include "UserInterface.h"
#include "ThreadLocal.h"

#include "Font.h"
#include "UIText.h"

#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "Cube.h"
#include "Camera.h"
#include "VoxelMap.h"

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

void GameLoop()
{
	Window window{ { 800, 600, false, "Test" } };

	SDL_Event evt;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	glm::mat4 projection = glm::perspective(70.0, (double)800 / 600, 1.0, 10000.0);
	glm::mat4 modelview;

	UIFont bebasFont("Fonts/bebas.ttf", 28);
	UITextPtr txt = UIText::Create(bebasFont);

	Cube c(1.0f);
	Camera camera;

	VoxelMap map;
	map.SpawnChunk(0, 0);

	UserInterface ui;
	ui.AddChild(txt);

	while (true)
	{
		TaskManager::GetInstance().Run(10);

		std::ostringstream os;
		os << "Background : " << TaskManager::GetInstance().GetBackgroundTasks()
			<< " Mainthread : " << TaskManager::GetInstance().GetBackgroundTasks();

		txt->SetText(os.str());

		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_WINDOWEVENT:
				if (evt.window.event == SDL_WINDOWEVENT_CLOSE)
					return;
				break;
			case SDL_KEYDOWN:
				if (evt.key.keysym.sym == SDLK_ESCAPE)
				{
					return;
				}
				if (evt.key.keysym.sym == SDLK_SPACE)
				{
					map.Revert();
				}
				else if (evt.key.keysym.sym == SDLK_w)
				{
					camera.Forward(1.0f);
				}
				else if (evt.key.keysym.sym == SDLK_s)
				{
					camera.Forward(-1.0f);
				}
				else if (evt.key.keysym.sym == SDLK_d)
				{
					camera.Strafe(1.0f);
				}
				else if (evt.key.keysym.sym == SDLK_a)
				{
					camera.Strafe(-1.0f);
				}
				break;
			case SDL_MOUSEMOTION:
				camera.Yaw(-evt.motion.xrel / 10.f);
				camera.Pitch(-evt.motion.yrel / 10.f);
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);

		camera.LookAt(modelview);

		c.Draw(projection, modelview);
		map.Draw(projection, modelview);
		
		ui.Draw(800.f, 600.f);

		window.Show();
	}
}