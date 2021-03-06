#pragma once

class UIFont
{
public:

	UIFont(const std::string& acPath, uint32_t aPointSize);
	~UIFont();

	TTF_Font* GetTTF() const;

private:

	TTF_Font* m_pFont{ nullptr };
};