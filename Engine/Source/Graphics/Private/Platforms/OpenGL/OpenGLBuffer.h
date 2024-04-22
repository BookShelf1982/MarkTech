#pragma once
#include "Buffer.h"
#include <glad\glad.h>

class COpenGLBuffer : public IBuffer
{
public:
	COpenGLBuffer();
	COpenGLBuffer(MCreateBufferInfo info);
	~COpenGLBuffer();
	uint32_t GetId() { return m_nId; }
private:
	uint32_t m_nId;
};

class COpenGLVertexBuffer : public IBuffer
{
	COpenGLVertexBuffer(MCreateBufferInfo info);
	~COpenGLVertexBuffer();

	uint32_t Getd() { return m_nId; }
private:
	uint32_t m_nId;
	uint32_t m_nVAOId;
};