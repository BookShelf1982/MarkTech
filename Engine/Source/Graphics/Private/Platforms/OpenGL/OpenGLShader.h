#pragma once
#include "Shader.h"
#include <glad\glad.h>

class COpenGLShader : public IShader
{
public:
	COpenGLShader(MCreateShaderInfo info);
	~COpenGLShader();

	uint32_t GetId() { return m_nId; }
private:
	uint32_t m_nId;

	uint32_t CreateShader(MCreateShaderInfo info);
};