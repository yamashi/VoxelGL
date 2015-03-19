#include "Window.h"
#include "Log.h"

#include "SDL.h"


Window::Window(const Options& acOptions)
{
	uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	if (acOptions.Fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	m_pWindow = SDL_CreateWindow(acOptions.Caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, acOptions.Width, acOptions.Height, flags);

	auto ctx = SDL_GL_CreateContext(m_pWindow);
	if (ctx == nullptr)
	{
		Crash("Unable to create a OpenGL context");
	}

	m_renderer.Initialize(ctx);


}

Window::~Window()
{
	SDL_DestroyWindow(m_pWindow);
}

void Window::Show()
{
	SDL_GL_SwapWindow(m_pWindow);
}