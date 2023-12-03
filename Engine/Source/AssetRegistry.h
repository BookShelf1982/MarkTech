#pragma once
#include "Core.h"

namespace MarkTech
{
	enum MARKTECH_API EAssetType
	{
		MUnknown,
		MTexture,
		MModel,
		MShader,
		MMaterial
	};

	struct MARKTECH_API MAssetData
	{
		uint64_t nId;
		EAssetType Type;
		void* pData;
		size_t nDataSize;
	};

	MAKE_CTARRAY_COMPATIBLE(MAssetData);
	MAKE_CTARRAY_COMPATIBLE(MAssetData*);

	class MARKTECH_API CAssetHandle
	{
	public:
		CAssetHandle();

		void Initialize(uint64_t assetId);

		MAssetData* GetAssetDataPtr();

	private:
		uint64_t m_nAssetId;
		MAssetData* m_pAsset;
	};

	class MARKTECH_API CAssetRegistry
	{
	public:
		CAssetRegistry();
		~CAssetRegistry();

		uint64_t LoadAsset(const char* filepath, EAssetType type);
		MAssetData* FindAssetById(uint64_t id);
		uint64_t GetAssetId(const char* filepath);
	private:
		MAssetData LoadTexture(const char* filepath);

		CTArray<MAssetData> m_RegisteredAssets;
	};
}