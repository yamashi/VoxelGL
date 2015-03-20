#pragma once

#include "UIFont.h"
#include "Shader.h"

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

	void Draw();

protected:

	void GenerateTexture();

private:

	UIFont& m_font;
	std::string m_text;
	SDL_Surface* m_pSurface{ nullptr };
	uint32_t m_textureId{ 0 };
	Shader m_simpleSplat;

	float m_height, m_width;
};