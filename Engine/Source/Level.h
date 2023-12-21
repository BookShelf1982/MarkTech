#pragma once
#include "DllExport.h"
#include "Core.h"
#include "AssetRegistry.h"
#include "ComponentRegistry.h"
#include "BaseEntity.h"

namespace MarkTech
{
	class MARKTECH_API CLevel
	{
	public:
		CLevel();
		~CLevel();

		bool InitLevel();

		void DestroyLevel();

		void UpdateLevel(float flDeltaTime);

		CAssetRegistry* GetAssetRegistry() { return &m_AssetRegistry; }

		CAssetHandle LoadAsset(const char* filepath, EAssetType type);
		
		template<class T>
		bool HasComponent(uint64_t id)
		{
			return m_Comps.ComponentExists<T>(id);
		}

		template<class T>
		void CreateComponent(uint64_t ownerId)
		{
			m_Comps.CreateComponent<T>(ownerId);
		}

		template<class T>
		T* GetComponentFromEntity(uint64_t entId)
		{
			return m_Comps.GetComponentFromOwner<T>(entId);
		}
		
		CBaseEntity* GetEntityById(uint64_t id);
		
		uint64_t CreateEntity();

	private:
		CTArray<CBaseEntity> m_Ents;
		CComponentRegistry m_Comps;

		CAssetRegistry m_AssetRegistry;
	};
}