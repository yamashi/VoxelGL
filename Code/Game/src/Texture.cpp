#include "Texture.h"
#include "Utils.h"
#include "Log.h"

Texture::Texture()
	: Initializable()
{
}

Texture::~Texture()
{
	if (glIsTexture(m_glTexture))
	{
		glDeleteTextures(1, &m_glTexture);
	}
}

bool Texture::Initialize(SDL_Surface* apSurface)
{
	if (apSurface == NULL)
	{
		return false;
	}

	uint32_t w = NextPower2(apSurface->w);
	uint32_t h = NextPower2(apSurface->h);

	SDL_Surface* inter = SDL_CreateRGBSurface(0, w, h, 32,
		0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	if (inter == NULL)
	{
		return false;
	}

	SDL_BlitSurface(apSurface, 0, inter, 0);

	if (!glIsTexture(m_glTexture))
	{
		glGenTextures(1, &m_glTexture);
	}

	glBindTexture(GL_TEXTURE_2D, m_glTexture);

	GLenum formatInterne(0);
	GLenum format(0);

	if (inter->format->BytesPerPixel == 3)
	{
		formatInterne = GL_RGB;

		if (inter->format->Rmask == 0xff)
			format = GL_RGB;

		else
			format = GL_BGR;
	}
	else if (inter->format->BytesPerPixel == 4)
	{
		formatInterne = GL_RGBA;

		if (inter->format->Rmask == 0xff)
			format = GL_RGBA;

		else
			format = GL_BGRA;
	}
	else
	{
		SDL_FreeSurface(inter);
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, inter->w, inter->h, 0, format, GL_UNSIGNED_BYTE, inter->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(inter);

	m_height = h;
	m_width = w;

	m_flags &= ~KDirty;

	return true;
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}