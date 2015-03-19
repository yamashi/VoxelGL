#pragma once

#ifdef WIN32
#	include <GL/glew.h>
#elif __APPLE__
#	define GL3_PROTOTYPES 1
#	include <OpenGL/gl3.h>
#else
#	define GL3_PROTOTYPES 1
#	include <GL3/gl3.h>
#endif

#include <iostream>
#include <string>
#include <fstream>

class Shader
{
public:

	Shader();
	Shader(const std::string& acVertCode, const std::string& acFragCode);
	~Shader();

	GLuint GetId() const;

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
