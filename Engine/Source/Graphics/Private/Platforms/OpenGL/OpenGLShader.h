#pragma once
#include "Shader.h"
#include <glad\glad.h>

class COpenGLShader : public IShader
{
public:
	COpenGLShader();
	COpenGLShader(MCreateShaderInfo info);
	~COpenGLShader();

private:
	uint32_t m_nId;

	uint32_t CreateShader(MCreateShaderInfo info);
};