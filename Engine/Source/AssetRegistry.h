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

	class MARKTECH_API CAssetObject
	{
	public:
		CAssetObject();
		virtual ~CAssetObject();

		virtual void Release();

		uint64_t m_nAssetId;
	};

	MAKE_CTARRAY_COMPATIBLE(CAssetObject);
	MAKE_CTARRAY_COMPATIBLE(CAssetObject*);

	class MARKTECH_API CAssetHandle
	{
	public:
		CAssetHandle();

		void Initialize(uint64_t assetId);

		CAssetObject* GetAssetDataPtr();

	private:
		uint64_t m_nAssetId;
		CAssetObject* m_pAsset;
	};

	class MARKTECH_API CAssetRegistry
	{
	public:
		CAssetRegistry();
		~CAssetRegistry();

		uint64_t LoadAsset(const char* filepath, EAssetType type);
		CAssetObject* FindAssetById(uint64_t id);
		uint64_t GetAssetId(const char* filepath);
	private:
		//MAssetData LoadTexture(const char* filepath);

		CTArray<CAssetObject*> m_RegisteredAssets;
	};
}