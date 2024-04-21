#include "ShaderResourceImpl.h"

CShaderResource::CShaderResource(uint32_t* ptr, uint32_t nSizeInBytes)
	:m_ptr(ptr), m_nSizeInBytes(nSizeInBytes)
{
}

CShaderResource::~CShaderResource()
{
	delete[] m_ptr;
}
