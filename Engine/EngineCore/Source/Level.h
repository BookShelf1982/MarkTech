#pragma once
#include "Core.h"
#include "AssetRegistry.h"
#include "ComponentRegistry.h"
#include "VesselRegistry.h"

class CLevel
{
public:
	CLevel();
	~CLevel();

	bool InitLevel();
	void UpdateLevel(float flDeltaTime);
	void DestroyLevel();

	uint64_t CreateVessel();

	template<class T>
	uint64_t AddComponentToVessel(uint64_t id);

	uint64_t LoadModelAsset(String path);
	MModelAsset* GetModelAsset(uint64_t assetId);
	uint64_t LoadShaderAsset(String path);
	MShaderAsset* GetShaderAsset(uint64_t assetId);

private:
	CAssetRegistry m_AssetRegistry;
	CVesselRegistry m_Vessels;
	CComponentRegistry m_Components;
};