#pragma once

#include "Initializable.h"

class Texture : public Initializable
{
public:

	Texture();
	~Texture();

	bool Initialize(SDL_Surface* apSurface);

	uint16_t GetWidth() const { return m_width; };
	uint16_t GetHeight() const { return m_height; };

	void Bind();
	void Unbind();

	static std::shared_ptr < Texture > Create() { return std::make_shared<Texture>(); }

private:

	uint16_t m_width{ 0 };
	uint16_t m_height{ 0 };
	GLuint m_glTexture{ 0 };

};

using TexturePtr = std::shared_ptr < Texture > ;
using TextureWeakPtr = std::weak_ptr < Texture > ;