#include "RenderList.h"

RenderList::RenderList()
{
}

RenderList::~RenderList()
{
	if (glIsVertexArray(m_vaoId))
		glDeleteVertexArrays(1, &m_vaoId);
}

void RenderList::Setup()
{
	if (glIsVertexArray(m_vaoId))
		glDeleteVertexArrays(1, &m_vaoId);

	glGenVertexArrays(1, &m_vaoId);
}

void RenderList::Bind()
{
	glBindVertexArray(m_vaoId);
}

void RenderList::Unbind()
{
	glBindVertexArray(0);
}