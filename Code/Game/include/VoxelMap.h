#pragma once

#include "VoxelChunk.h"
#include "Shader.h"

class VoxelMap
{
public:

	void SpawnChunk(uint32_t aX, uint32_t aZ);

	void Draw(const glm::mat4& aProjection, const glm::mat4& aModelView);

	void Revert() { m_pCurrent->Revert(); }

private:

	GLuint m_projectionIndex;
	GLuint m_modelViewIndex;
	GLuint m_mvpIndex;

	ShaderPtr m_pColorShader;

	VoxelChunkPtr m_pCurrent; // temp
};