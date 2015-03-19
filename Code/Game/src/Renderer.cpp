#include "Renderer.h"
#include "Log.h"

#include "SDL.h"

#ifdef WIN32
#	include <GL/glew.h>
#elif __APPLE__
#	define GL3_PROTOTYPES 1
#	include <OpenGL/gl3.h>
#else
#	define GL3_PROTOTYPES 1
#	include <GL3/gl3.h>
#endif

void InitializeRenderer()
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		Crash("Failed to initialize SDL");
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

void Renderer::Initialize(SDL_GLContext aContext)
{
	m_glContext = aContext;

#ifdef WIN32
	if (glewInit() != GLEW_OK)
	{
		Crash("Failed to initialize GLEW");
	}
#endif
}