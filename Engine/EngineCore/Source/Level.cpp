#include "Level.h"

CLevel::CLevel()
{
}

CLevel::~CLevel()
{
}

bool CLevel::InitLevel()
{
	m_Components.InitComponentRegistry();
	return true;
}

void CLevel::UpdateLevel(float flDeltaTime)
{
	m_Components.UpdateComponentRegistry(flDeltaTime);
}

void CLevel::DestroyLevel()
{
	m_Components.DestroyComponentRegistry();
	m_AssetRegistry.DestroyAssetRegistry();
}

uint64_t CLevel::LoadModelAsset(String path)
{
	return m_AssetRegistry.LoadModelAsset(path);
}

MModelAsset* CLevel::GetModelAsset(uint64_t assetId)
{
	return m_AssetRegistry.GetModelAsset(assetId);
}

uint64_t CLevel::LoadShaderAsset(String path)
{
	return m_AssetRegistry.LoadShaderAsset(path);
}

MShaderAsset* CLevel::GetShaderAsset(uint64_t assetId)
{
	return m_AssetRegistry.GetShaderAsset(assetId);
}

