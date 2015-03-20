#pragma once

void InitializeRenderer();
void ShutdownRenderer();

class Renderer
{
public:

	Renderer();
	~Renderer();

	void Initialize(SDL_GLContext aContext);

private:

	SDL_GLContext m_glContext{ nullptr };
};