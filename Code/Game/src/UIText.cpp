#include "UIText.h"
#include "Color.h"
#include "Utils.h"
#include "ShaderManager.h"

UIText::UIText(UIFont& aFont)
	: m_font(aFont)
	, m_pShader(ShaderManager::GetInstance().Get("ui_texture"))
{
	DoConstruct();
}

UIText::UIText(const UIText& aRhs)
	: m_font(aRhs.m_font)
	, m_text(aRhs.m_text)
	, m_pShader(ShaderManager::GetInstance().Get("ui_texture"))
{
	DoConstruct();
}

void UIText::DoConstruct()
{
	if (m_pShader->IsDirty())
	{
		m_pShader->Load("Shaders/Splat.vert", "Shaders/Splat.frag");
	}
}

UIText::~UIText()
{
	if (glIsVertexArray(m_vaoId))
		glDeleteVertexArrays(1, &m_vaoId);
	
	if (glIsBuffer(m_vboId))
		glDeleteBuffers(1, &m_vboId);
}

UIText& UIText::operator=(const UIText& aRhs)
{
	m_font = aRhs.m_font;
	m_text = aRhs.m_text;

	return *this;
}

void UIText::SetFont(UIFont& acFont)
{
	m_font = acFont;
	GenerateTexture();
}

void UIText::SetText(const std::string& acText)
{
	m_text = acText;
	GenerateTexture();
}

std::string UIText::GetText() const
{
	return m_text;
}

void UIText::Draw(float aScreenWidth, float aScreenHeight, glm::mat4 aProjection, glm::mat4 aModelView)
{
	const uint32_t cSizeVertices = sizeof(float) * 8;
	const uint32_t cSizeCoords = sizeof(float) * 8;

	float w = float(m_texture.GetWidth());
	float h = float(m_texture.GetHeight());

	aModelView = glm::mat4(1.0);
	aModelView = glm::translate(aModelView, glm::vec3(0.0f + m_posX, 0.0f + m_posY, 0.0f));

	glUseProgram(m_pShader->GetId());
	{

		glBindVertexArray(m_vaoId);
		{
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "modelview"), 1, GL_FALSE, value_ptr(aModelView));
			glUniformMatrix4fv(glGetUniformLocation(m_pShader->GetId(), "projection"), 1, GL_FALSE, value_ptr(aProjection));

			m_texture.Bind();
			{
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			m_texture.Unbind();
		}
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

void UIText::GenerateTexture()
{
	SDL_Color color = { 255, 255, 0, 0 };

	SDL_Surface* initial = TTF_RenderText_Blended(m_font.GetTTF(), m_text.c_str(), color);

	m_texture.Initialize(initial);

	SDL_FreeSurface(initial);

	float vertices[] =
	{
		0, 0 ,
		m_texture.GetWidth(), 0 ,
		m_texture.GetWidth(), m_texture.GetHeight(),
		m_texture.GetWidth(), m_texture.GetHeight(),
		0, 0,
		0, m_texture.GetHeight()
	};

	float coordTexture[] = {
		0, 0,
		1, 0,
		1, 1,
		1, 1,
		0, 0,
		0, 1
	};

	const uint32_t cSizeVertices = sizeof(float) * 12;
	const uint32_t cSizeCoords = sizeof(float) * 12;

	if (glIsVertexArray(m_vaoId))
		glDeleteVertexArrays(1, &m_vaoId);

	if (glIsBuffer(m_vboId))
		glDeleteBuffers(1, &m_vboId);

	glGenBuffers(1, &m_vboId);
	glGenVertexArrays(1, &m_vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	{
		glBufferData(GL_ARRAY_BUFFER, cSizeVertices + cSizeCoords, 0, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, cSizeVertices, vertices);
		glBufferSubData(GL_ARRAY_BUFFER, cSizeVertices, cSizeCoords, coordTexture);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(m_vaoId);
	{

		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		{
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cSizeVertices));
			glEnableVertexAttribArray(2);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}