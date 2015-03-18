#include "Window.h"
#include "SDL.h"

#include "easylogging++.h"

Window::Window(const Options& acOptions)
{
	uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	if (acOptions.Fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	m_pWindow = SDL_CreateWindow(acOptions.Caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, acOptions.Width, acOptions.Height, flags);

	m_renderer.SetContext(SDL_GL_CreateContext(m_pWindow));
}

Window::~Window()
{
	SDL_DestroyWindow(m_pWindow);
}