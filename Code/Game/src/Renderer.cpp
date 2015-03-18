#include "Renderer.h"

#include "SDL.h"

#include "easylogging++.h"

void InitializeRenderer()
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		LOG(ERROR) << "error=\"Failed to initialize SDL.\"";
		exit(-1);
	}
}

void ShutdownRenderer()
{
	SDL_Quit();
}

Renderer::Renderer()
{
	// Version d'OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	LOG(INFO) << "msg=\"Renderer initialized\"";
}

Renderer::~Renderer()
{
	SDL_GL_DeleteContext(m_glContext);

	LOG(INFO) << "msg=\"Renderer destroyed\"";
}

void Renderer::SetContext(SDL_GLContext aContext)
{
	m_glContext = aContext;
}