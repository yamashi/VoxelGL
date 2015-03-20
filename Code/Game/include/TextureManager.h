#pragma once

#include "Texture.h"
#include "IManager.h"

class TextureManager : public IManager<Texture>
{
public:

	TextureManager() : IManager<Texture>(500){}
	~TextureManager() {}

	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}
};