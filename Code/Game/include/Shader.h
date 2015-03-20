#pragma once

#include "Initializable.h"

class Shader : public Initializable
{
public:

	Shader();
	~Shader();

	GLuint GetId() const;

	void Load(const std::string& acVertCode, const std::string& acFragCode);

	static std::shared_ptr<Shader> Create() { return std::make_shared<Shader>(); }

protected:

	bool Load();
	bool Build(GLuint &shader, GLenum type, std::string const &fichierSource);

private:

	GLuint m_vertexId{ 0 };
	GLuint m_fragmentId{ 0 };
	GLuint m_programId{ 0 };

	std::string m_vertexSource;
	std::string m_fragmentSource;
};

using ShaderPtr = std::shared_ptr < Shader > ;
using ShaderWeakPtr = std::weak_ptr < Shader > ;