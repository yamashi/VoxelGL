#include "Mesh.h"
#include "Utils.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	if (glIsBuffer(m_vboId))
		glDeleteBuffers(1, &m_vboId);
}

void Mesh::Set(const std::vector<float>& acVertices, const std::vector<float>& acColors, const std::vector<float>& acTextureCoords)
{
	m_verticesSize = acVertices.size() * sizeof(float);
	m_colorsSize = acColors.size() * sizeof(float);
	m_textureCoordsSize = acTextureCoords.size() * sizeof(float);

	uint32_t totalSize = m_verticesSize + m_colorsSize + m_textureCoordsSize;

	if (glIsBuffer(m_vboId))
		glDeleteBuffers(1, &m_vboId);

	glGenBuffers(1, &m_vboId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	{
		glBufferData(GL_ARRAY_BUFFER, totalSize, 0, GL_STATIC_DRAW);

		if (m_verticesSize)
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_verticesSize, acVertices.data());
		
		if (m_colorsSize)
			glBufferSubData(GL_ARRAY_BUFFER, m_colorsSize, m_verticesSize, acColors.data());
		
		if (m_textureCoordsSize)
			glBufferSubData(GL_ARRAY_BUFFER, m_textureCoordsSize, m_verticesSize + m_colorsSize, acTextureCoords.data());
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
}

void Mesh::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Enable(Type aType, uint32_t aIndex) const
{
	uint32_t offset = 0;

	switch (aType)
	{
	case KVertices:
		break;
	case KColors:
		offset = m_verticesSize;
		break;
	case KTextureCoords:
		offset = m_verticesSize + m_colorsSize;
		break;
	}

	glVertexAttribPointer(aIndex, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(aIndex);
}