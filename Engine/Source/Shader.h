#pragma once
#include "Core.h"
#include "Level.h"

namespace MarkTech
{
	class MARKTECH_API CShader : public CAssetObject
	{
	public:
		CShader();
		virtual ~CShader() override;
		
		virtual void Release() override;

		CTMemoryBlob<char> m_pShaderByteCode;
		size_t m_nShaderByteCodeSize;
	};

	CShader* LoadShader(const char* filepath);
}