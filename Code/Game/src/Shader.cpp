#include "Shader.h"
#include "Log.h"
#include "Memory.h"

Shader::Shader()
{
}

Shader::Shader(const std::string& acVertCode, const std::string& acFragCode)
{
	m_vertexSource = acVertCode;
	m_fragmentSource = acFragCode;

	Load();
}


Shader::~Shader()
{
    glDeleteShader(m_vertexId);
    glDeleteShader(m_fragmentId);
    glDeleteProgram(m_programId);
}

bool Shader::Load()
{
    if(glIsShader(m_vertexId) == GL_TRUE)
        glDeleteShader(m_vertexId);

    if(glIsShader(m_fragmentId) == GL_TRUE)
        glDeleteShader(m_fragmentId);

    if(glIsProgram(m_programId) == GL_TRUE)
        glDeleteProgram(m_programId);


    if(!Build(m_vertexId, GL_VERTEX_SHADER, m_vertexSource))
        return false;

    if(!Build(m_fragmentId, GL_FRAGMENT_SHADER, m_fragmentSource))
        return false;


    m_programId = glCreateProgram();


    glAttachShader(m_programId, m_vertexId);
    glAttachShader(m_programId, m_fragmentId);


    glBindAttribLocation(m_programId, 0, "in_Vertex");
    glBindAttribLocation(m_programId, 1, "in_Color");
    glBindAttribLocation(m_programId, 2, "in_TexCoord0");


    glLinkProgram(m_programId);

	GLint erreurLink{ 0 };
    glGetProgramiv(m_programId, GL_LINK_STATUS, &erreurLink);


    if(erreurLink != GL_TRUE)
    {
		GLint errorLen{ 0 };
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &errorLen);

		char *pError = StackAllocateArray(char, errorLen);

        glGetShaderInfoLog(m_programId, errorLen, &errorLen, pError);
        pError[errorLen] = '\0';

		LOG(ERROR) << "error=\"Link failed\" msg=\"" << pError << "\"";

        glDeleteProgram(m_programId);

        return false;
    }

	return true;
}


bool Shader::Build(GLuint &shader, GLenum type, const std::string &filePath)
{
    shader = glCreateShader(type);
    if(shader == 0)
    {
		LOG(ERROR) << "error=\"No shader type exists\" type=" << type;
        return false;
    }

    std::ifstream src(filePath.c_str());
    if(!src)
    {
		LOG(ERROR) << "error=\"Can't find source file\" path=\"" << filePath << "\"";
        glDeleteShader(shader);

        return false;
    }

    std::string line;
    std::string sourceCode;

	while (getline(src, line))
		sourceCode += line + '\n';

    src.close();

	const GLchar* cpSource = sourceCode.c_str();

	glShaderSource(shader, 1, &cpSource, 0);

    glCompileShader(shader);

	GLint error{ 0 };
    glGetShaderiv(shader, GL_COMPILE_STATUS, &error);

    if(error != GL_TRUE)
    {
		GLint errorLen{ 0 };
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLen);

		char* pError = StackAllocateArray(char, errorLen);

		glGetShaderInfoLog(shader, errorLen, &errorLen, pError);
		pError[errorLen] = '\0';

		LOG(ERROR) << "error=\"Build failed\" msg=\"" << pError << "\"";

        glDeleteShader(shader);

        return false;
    }

	return true;
}

GLuint Shader::GetId() const
{
    return m_programId;
}
