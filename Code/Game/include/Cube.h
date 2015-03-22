#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "RenderList.h"

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
	RenderList m_renderList;
	Mesh m_mesh;
};