#pragma once

#include "SDL.h"

void InitializeRenderer();
void ShutdownRenderer();

class Renderer
{
public:

	Renderer();
	~Renderer();

	void SetContext(SDL_GLContext aContext);

private:

	SDL_GLContext m_glContext{ nullptr };
};