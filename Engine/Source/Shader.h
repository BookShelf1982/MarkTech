#pragma once
#include "Core.h"
#include "Level.h"

namespace MarkTech
{
	class MARKTECH_API CShader
	{
	public:
		CShader();
		~CShader();

		void Initialize(const void* bytecode, size_t size);
	protected:
		const void* m_pShaderByteCode;
		size_t m_nShaderByteCodeSize;
	};

	MAssetData LoadShader(const char* filepath);
}