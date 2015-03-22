#pragma once

class RenderList
{
public:

	RenderList();
	~RenderList();

	void Bind();
	void Unbind();

	void Setup();

private:

	GLuint m_vaoId{ 0 };
};