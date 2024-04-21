#include "OpenGLShader.h"
#include <vector>

COpenGLShader::COpenGLShader()
{
}

COpenGLShader::COpenGLShader(MCreateShaderInfo info)
{
	m_nId = CreateShader(info);
}

COpenGLShader::~COpenGLShader()
{
	glDeleteShader(m_nId);
}

uint32_t COpenGLShader::CreateShader(MCreateShaderInfo info)
{
	GLenum type;
	switch (info.stage)
	{
	case EShaderStage::VERTEX:
		type = GL_VERTEX_SHADER;
		break;
	case EShaderStage::FRAGMENT:
		type = GL_FRAGMENT_SHADER;
		break;
	default:
		type = GL_VERTEX_SHADER;
		break;
	}

	uint32_t shaderId = glCreateShader(type);

	glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, info.pShaderData, info.nShaderDataSize);

	glSpecializeShader(shaderId, info.pEntrypoint, 0, nullptr, nullptr);

	GLint isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		// We don't need the shader anymore.
		glDeleteShader(shaderId);

		MASSERT(false);

		// In this simple program, we'll just leave
		return 0;
	}

	return shaderId;
}
