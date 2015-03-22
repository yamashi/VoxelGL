#include "VoxelMap.h"
#include "ShaderManager.h"

void VoxelMap::SpawnChunk(uint32_t aX, uint32_t aZ)
{
	m_pColorShader = ShaderManager::GetInstance().Get("basic_color_3d");
	if (m_pColorShader->IsDirty())
	{
		m_pColorShader->Load("Shaders/test3D.vert", "Shaders/test3D.frag");
	}

	m_projectionIndex = glGetUniformLocation(m_pColorShader->GetId(), "projection");
	m_modelViewIndex = glGetUniformLocation(m_pColorShader->GetId(), "modelView");
	m_mvpIndex = glGetUniformLocation(m_pColorShader->GetId(), "mvp");

	m_pCurrent = VoxelChunk::CreateUpdate();
}

void VoxelMap::Draw(const glm::mat4& aProjection, const glm::mat4& aModelView)
{
	glUseProgram(m_pColorShader->GetId());

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_pCurrent->Draw(aProjection, aModelView, m_projectionIndex, m_modelViewIndex, m_mvpIndex);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(0);
}