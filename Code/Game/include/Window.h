#pragma once

#include "Renderer.h"

struct SDL_Window;
class Window
{
public:

	struct Options
	{
		uint32_t Width, Height;
		bool Fullscreen;
		std::string Caption;
	};

	Window(const Options& acOptions);
	~Window();

	void Show();

private:

	SDL_Window* m_pWindow{ nullptr };
	Renderer m_renderer;
};