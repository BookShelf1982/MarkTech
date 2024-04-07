#include "OpenGLBuffer.h"

COpenGLBuffer::COpenGLBuffer()
{
}

COpenGLBuffer::COpenGLBuffer(MCreateBufferInfo info)
{
	GLenum usage;
	GLenum target;

	switch (info.usage)
	{
	case EBufferUsage::STATIC:
		usage = GL_STATIC_DRAW;
		break;
	case EBufferUsage::DYNAMIC:
		usage = GL_DYNAMIC_DRAW;
		break;
	default:
		usage = GL_STATIC_DRAW;
		break;
	}

	switch (info.target)
	{
	case EBindingTarget::VERTEX_ARRAY:
		target = GL_ARRAY_BUFFER;
		break;
	case EBindingTarget::INDEX_ARRAY:
		target = GL_ELEMENT_ARRAY_BUFFER;
		break;
	case EBindingTarget::UNIFORM_BUFFER:
		target = GL_UNIFORM_BUFFER;
		break;
	default:
		target = GL_ARRAY_BUFFER;
		break;
	}

	glGenBuffers(1, &m_nId);
	glBindBuffer(GL_ARRAY_BUFFER, m_nId);
	glBufferData(GL_ARRAY_BUFFER, info.nSize, info.pData, usage);
}

COpenGLBuffer::~COpenGLBuffer()
{
	glDeleteBuffers(1, &m_nId);
}
