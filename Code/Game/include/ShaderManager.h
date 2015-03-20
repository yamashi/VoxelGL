#pragma once

#include "Shader.h"
#include "IManager.h"

class ShaderManager : public IManager<Shader>
{
public:

	ShaderManager() : IManager<Shader>(200){}
	~ShaderManager(){}

	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};