#include "Cube.h"
#include "ShaderManager.h"
#include "Utils.h"

using namespace glm;


Cube::Cube(float aSize) 
	: m_pShader(ShaderManager::GetInstance().Get("basic_color_3d"))
{
	if (m_pShader->IsDirty())
	{
		m_pShader->Load("Shaders/test3D.vert", "Shaders/test3D.frag");
	}

	aSize /= 2;

	Load(aSize);
}


Cube::~Cube()
{
	if (glIsBuffer(m_vboId) == GL_TRUE)
		glDeleteBuffers(1, &m_vboId);

	if (glIsVertexArray(m_vaoId) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoId);
}


void Cube::Load(float aSize)
{
	float vertices[] = { -aSize, -aSize, -aSize, aSize, -aSize, -aSize, aSize, aSize, -aSize,     // Face 1
		-aSize, -aSize, -aSize, -aSize, aSize, -aSize, aSize, aSize, -aSize,     // Face 1

		aSize, -aSize, aSize, aSize, -aSize, -aSize, aSize, aSize, -aSize,       // Face 2
		aSize, -aSize, aSize, aSize, aSize, aSize, aSize, aSize, -aSize,         // Face 2

		-aSize, -aSize, aSize, aSize, -aSize, aSize, aSize, -aSize, -aSize,      // Face 3
		-aSize, -aSize, aSize, -aSize, -aSize, -aSize, aSize, -aSize, -aSize,    // Face 3

		-aSize, -aSize, aSize, aSize, -aSize, aSize, aSize, aSize, aSize,        // Face 4
		-aSize, -aSize, aSize, -aSize, aSize, aSize, aSize, aSize, aSize,        // Face 4

		-aSize, -aSize, -aSize, -aSize, -aSize, aSize, -aSize, aSize, aSize,     // Face 5
		-aSize, -aSize, -aSize, -aSize, aSize, -aSize, -aSize, aSize, aSize,     // Face 5

		-aSize, aSize, aSize, aSize, aSize, aSize, aSize, aSize, -aSize,         // Face 6
		-aSize, aSize, aSize, -aSize, aSize, -aSize, aSize, aSize, -aSize };      // Face 6


	float colors[] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,           // Face 1
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,           // Face 1

		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,           // Face 2
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,           // Face 2

		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,           // Face 3
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,           // Face 3

		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,           // Face 4
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,           // Face 4

		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,           // Face 5
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,           // Face 5

		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,           // Face 6
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 };          // Face 6

	if (glIsBuffer(m_vboId) == GL_TRUE)
		glDeleteBuffers(1, &m_vboId);

	if (glIsVertexArray(m_vaoId) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoId);

	glGenBuffers(1, &m_vboId);

	glGenVertexArrays(1, &m_vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	{
		glBufferData(GL_ARRAY_BUFFER, cSizeVertices + cSizeColors, 0, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, cSizeVertices, vertices);
		glBufferSubData(GL_ARRAY_BUFFER, cSizeVertices, cSizeColors, colors);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_vaoId);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cSizeVertices));
			glEnableVertexAttribArray(1);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}


void Cube::Draw(glm::mat4 &aProjection, glm::mat4 &aModelView)
{
	glUseProgram(m_pShader->GetId());
	{
		glBindVertexArray(m_vaoId);
		{
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "projection"), 1, GL_FALSE, value_ptr(aProjection));
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "modelview"), 1, GL_FALSE, value_ptr(aModelView));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	}
	glUseProgram(0);
}

/*
void Cube::updateVBO(void *donnees, int tailleBytes, int decalage)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	void *adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (adresseVBO == NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return;
	}

	memcpy((char*)adresseVBO + decalage, donnees, tailleBytes);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	adresseVBO = 0;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
*/