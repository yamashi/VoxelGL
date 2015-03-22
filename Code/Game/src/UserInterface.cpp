#include "UserInterface.h"
#include "ShaderManager.h"

void InitializeUI()
{
	TTF_Init();
}

void ShutdownUI()
{
	TTF_Quit();
}

UserInterface::UserInterface()
	: m_pShader(ShaderManager::GetInstance().Get("ui_texture"))
{
	if (m_pShader->IsDirty())
	{
		m_pShader->Load("Shaders/Splat.vert", "Shaders/Splat.frag");
	}
	
	m_projectIndex = glGetUniformLocation(m_pShader->GetId(), "projection");
	m_modelViewIndex = glGetUniformLocation(m_pShader->GetId(), "modelView");
	m_mvpIndex = glGetUniformLocation(m_pShader->GetId(), "mvp");
}

UserInterface::~UserInterface()
{

}

void UserInterface::Draw(float aScreenWidth, float aScreenHeight)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 Projection = glm::ortho(0.0f, aScreenWidth, aScreenHeight, 0.0f, 0.0f, 100.0f);
	glm::mat4 ModelView = glm::mat4(1.0);
	glm::mat4 MVP = Projection * ModelView;

	glUseProgram(m_pShader->GetId());

	UIRenderContext ctx { Projection, ModelView, MVP, m_projectIndex, m_modelViewIndex, m_mvpIndex };

	m_root.Draw(ctx);

	glUseProgram(0);
}

void UserInterface::AddChild(const UIElementPtr& acElement)
{
	m_root.AddChild(acElement);
}

void UserInterface::RemoveChild(const UIElementPtr& acElement)
{
	m_root.RemoveChild(acElement);
}