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
}


void Cube::Load(float aSize)
{
	std::vector<float> vertices = { aSize, -aSize, -aSize, -aSize, -aSize, -aSize, aSize, aSize, -aSize,     // Face 1
		-aSize, -aSize, -aSize, -aSize, aSize, -aSize, aSize, aSize, -aSize,     // Face 1

		aSize, -aSize, aSize, aSize, -aSize, -aSize, aSize, aSize, -aSize,       // Face 2
		aSize, aSize, aSize, aSize, -aSize, aSize, aSize, aSize, -aSize,         // Face 2

		aSize, -aSize, aSize, -aSize, -aSize, aSize, aSize, -aSize, -aSize,      // Face 3
		-aSize, -aSize, aSize, -aSize, -aSize, -aSize, aSize, -aSize, -aSize,    // Face 3

		-aSize, -aSize, aSize, aSize, -aSize, aSize, aSize, aSize, aSize,        // Face 4
		-aSize, aSize, aSize, -aSize, -aSize, aSize, aSize, aSize, aSize,        // Face 4

		-aSize, -aSize, -aSize, -aSize, -aSize, aSize, -aSize, aSize, aSize,     // Face 5
		-aSize, aSize, -aSize, -aSize, -aSize, -aSize, -aSize, aSize, aSize,     // Face 5

		-aSize, aSize, aSize, aSize, aSize, aSize, aSize, aSize, -aSize,         // Face 6
		-aSize, aSize, -aSize, -aSize, aSize, aSize, aSize, aSize, -aSize };      // Face 6

	std::vector<float> colors = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,           // Face 1
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

	m_mesh.Set(vertices, colors, {});

	m_renderList.Setup();

	m_renderList.Bind();
	{
		m_mesh.Bind();
		{
			m_mesh.Enable(Mesh::KVertices, 0);
			m_mesh.Enable(Mesh::KColors, 1);
		}
		m_mesh.Unbind();
	}
	m_renderList.Unbind();
}


void Cube::Draw(glm::mat4 &aProjection, glm::mat4 &aModelView)
{
	glUseProgram(m_pShader->GetId());
	{
		m_renderList.Bind();
		{
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "projection"), 1, GL_FALSE, value_ptr(aProjection));
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "modelview"), 1, GL_FALSE, value_ptr(aModelView));
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "mvp"), 1, GL_FALSE, value_ptr(aProjection * aModelView));

			glDrawArrays(GL_TRIANGLES, 0, m_mesh.GetVerticesCount());
		}
		m_renderList.Unbind();
	}
	glUseProgram(0);
}
