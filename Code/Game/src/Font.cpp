#include "Font.h"

UIFont::UIFont(const std::string& acPath, uint32_t aPointSize)
{
	m_pFont = TTF_OpenFont(acPath.c_str(), aPointSize);
}

UIFont::~UIFont()
{
	TTF_CloseFont(m_pFont);
}

TTF_Font* UIFont::GetTTF() const
{
	return m_pFont;
}