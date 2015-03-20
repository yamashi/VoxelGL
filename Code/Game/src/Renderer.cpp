#include "Renderer.h"
#include "Log.h"


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
#ifdef __APPLE__
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

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

	SDL_GL_SetSwapInterval(0);

	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);

#ifdef WIN32
	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK)
	{
		Crash("Failed to initialize GLEW");
	}
#endif
}