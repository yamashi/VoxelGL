#pragma once

#include "Shader.h"

class Cube
{
public:

	Cube(float aSize);
	~Cube();

	void Draw(glm::mat4 &aProjection, glm::mat4 &aModelView);

	const uint32_t cSizeVertices = 108 * sizeof(float);
	const uint32_t cSizeColors = 108 * sizeof(float);

private:

	void Load(float aSize);

protected:

	ShaderPtr m_pShader;
	GLuint m_vboId{ 0 };
	GLuint m_vaoId{ 0 };
};