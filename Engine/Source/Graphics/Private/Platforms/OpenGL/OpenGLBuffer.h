#pragma once
#include "Buffer.h"
#include <glad\glad.h>

class COpenGLBuffer : public IBuffer
{
public:
	COpenGLBuffer();
	COpenGLBuffer(MCreateBufferInfo info);
	~COpenGLBuffer();
private:
	uint32_t m_nId;
};