#pragma once
#include "Core.h"

namespace MarkTech
{
	enum MEAssetType
	{
		METexture,
		MEModel,
	};

	class CAssetData
	{
	public:
		uint64_t nAssetId;
		MEAssetType Type;
		void* pData;
	};

	MAKE_CTARRAY_COMPATIBLE(CAssetData);
	MAKE_CTARRAY_COMPATIBLE(CAssetData*);

	template<class T>
	class CTAssetReadWrite
	{
	public:

	private:
		T m_Asset;
	};

	class CAssetRegistry
	{
	public:
		CAssetRegistry();
		~CAssetRegistry();

		bool LoadAsset(const char* filepath);
	private:
		CTArray<CAssetData*> m_RegisteredAssets;
	};

	enum METextureCompression
	{
		MENormal,
		MEDXT1,
		MEDXT5
	};

	class CTextureAssetData : public CAssetData
	{
	public:
		int nWidth;
		int nHeight;
		int nChannels;
		METextureCompression Compression;
	};
}