#include "UIText.h"
#include "Color.h"
#include "Utils.h"

#ifdef WIN32
#	include <GL/glew.h>
#elif __APPLE__
#	define GL3_PROTOTYPES 1
#	include <OpenGL/gl3.h>
#else
#	define GL3_PROTOTYPES 1
#	include <GL3/gl3.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

UIText::UIText(UIFont& aFont)
	: m_font(aFont)
	, m_simpleSplat("Shaders/Splat.vert", "Shaders/Splat.frag")
{

}

UIText::UIText(const UIText& aRhs)
	: m_font(aRhs.m_font)
	, m_text(aRhs.m_text)
{

}

UIText::~UIText()
{

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

void UIText::Draw()
{
	float vertices[] = {
		-1, 1,
		-1 + m_width, 1,
		-1 + m_width, 1 - m_height,
		-1, 1 - m_height
	};

	float coordTexture[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	glm::mat4 projection;
	glm::mat4 modelview;

	projection = glm::perspective(70.0, (double)800 / 600, 1.0, 100.0);
	modelview = glm::mat4(1.0);

	glUseProgram(m_simpleSplat.GetId());

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, coordTexture);
	glEnableVertexAttribArray(2);

	glUniformMatrix4fv(glGetUniformLocation(m_simpleSplat.GetId(), "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_simpleSplat.GetId(), "modelview"), 1, GL_FALSE, value_ptr(modelview));

	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glDrawArrays(GL_QUADS, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);

	glUseProgram(0);
}

void UIText::GenerateTexture()
{
	SDL_Color color = { 255, 255, 0, 0 };

	SDL_Surface* initial = TTF_RenderText_Blended(m_font.GetTTF(), m_text.c_str(), color);

	uint32_t w = NextPower2(initial->w);
	uint32_t h = NextPower2(initial->h);

	SDL_Surface* inter = SDL_CreateRGBSurface(0, w, h, 32,
		0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, inter, 0);

	m_height = float(h) / 800.f;
	m_width = float(w) / 600.f;

	SDL_FreeSurface(initial);

	m_pSurface = inter;

	glGenTextures(1, &m_textureId);

	glBindTexture(GL_TEXTURE_2D, m_textureId);

	GLenum formatInterne(0);
	GLenum format(0);

	if (m_pSurface->format->BytesPerPixel == 3)
	{
		formatInterne = GL_RGB;

		if (m_pSurface->format->Rmask == 0xff)
			format = GL_RGB;

		else
			format = GL_BGR;
	}
	else if (m_pSurface->format->BytesPerPixel == 4)
	{
		formatInterne = GL_RGBA;

		if (m_pSurface->format->Rmask == 0xff)
			format = GL_RGBA;

		else
			format = GL_BGRA;
	}
	else
	{
		SDL_FreeSurface(m_pSurface);
		return;
	}


	glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, m_pSurface->w, m_pSurface->h, 0, format, GL_UNSIGNED_BYTE, m_pSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}