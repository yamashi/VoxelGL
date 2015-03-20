#pragma once

#include "UIFont.h"
#include "Shader.h"
#include "Texture.h"

class UIText
{
public:

	UIText(UIFont& aFont);
	UIText(const UIText& aRhs);
	~UIText();

	UIText& operator=(const UIText& aRhs);

	void SetFont(UIFont& acFont);
	void SetText(const std::string& acText);

	std::string GetText() const;

	void Draw(float aScreenWidth, float aScreenHeight, glm::mat4 aProjection, glm::mat4 aModelView);

	void SetPosition(float aX, float aY) { m_posX = aX; m_posY = aY; }
	float GetPositionX() const { return m_posX; }
	float GetPositionY() const { return m_posY; }

protected:

	void GenerateTexture();

private:

	void DoConstruct();

	GLuint m_vboId{ 0 };
	GLuint m_vaoId{ 0 };

	float m_posX{ 0.0f };
	float m_posY{ 0.0f };

	UIFont& m_font;
	std::string m_text;
	ShaderPtr m_pShader;
	Texture m_texture;
};