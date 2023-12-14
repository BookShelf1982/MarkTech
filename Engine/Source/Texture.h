#pragma once
#include "Core.h"
#include "Level.h"

namespace MarkTech
{
	class MARKTECH_API CTexture : public CAssetObject
	{
	public:
		CTexture();
		~CTexture();

		virtual void Release() override;

		CTMemoryBlob<char> m_pImgData;
		size_t m_nImgDataSize;
	};

	CTexture* LoadTexture(const char* filepath);
}