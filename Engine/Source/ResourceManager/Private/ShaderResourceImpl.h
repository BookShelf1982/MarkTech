#pragma once
#include "ShaderResource.h"

class CShaderResource : public IShaderResource
{
public:
	CShaderResource(uint32_t* ptr, uint32_t nSizeInBytes);
	~CShaderResource();

	virtual uint32_t* GetPtr() override { return m_ptr; }
	virtual uint32_t GetSizeInBytes() override { return m_nSizeInBytes; }
private:
	uint32_t* m_ptr;
	uint32_t m_nSizeInBytes;
};