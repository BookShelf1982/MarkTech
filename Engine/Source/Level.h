#pragma once
#include "DllExport.h"
#include "Core.h"
#include "AssetRegistry.h"
#include "BaseEntity.h"

namespace MarkTech
{
	class MARKTECH_API CLevel
	{
	public:
		bool InitLevel();

		void DestroyLevel();

		void UpdateLevel(float flDeltaTime);

		static CLevel* GetLevel() { return g_pLevel; }

		CAssetRegistry* GetAssetRegistry() { return &m_AssetRegistry; }

		CAssetHandle LoadAsset(const char* filepath, EAssetType type);
		
		template<class T>
		bool HasComponent(uint64_t id)
		{
			for (int i = 0; i < m_Comps.GetSize(); i++)
			{
				//Is owned by specified entity
				if (m_Comps[i]->GetOwnerId() == id)
				{
					//Is specified type
					auto comp = dynamic_cast<T*>(m_Comps[i]);
					if (comp != nullptr)
					{
						return true;
					}
				}
			}
			//return false if we can't find it
			return false;
		}

		template<class T>
		void CreateComponent(uint64_t ownerId)
		{
			T* comp = new T(ownerId);
			comp->InitComponent();
			m_Comps.Push(comp);
		}

		template<class T>
		T* GetComponentFromEntity(uint64_t entId)
		{
			for (int i = 0; i < m_Comps.GetSize(); i++)
			{
				if (m_Comps.c_arr()[i]->GetOwnerId() == entId)
				{
					return dynamic_cast<T*>(m_Comps.c_arr()[i]);
				}
			}
			return nullptr;
		}

		CBaseEntity* GetEntityById(uint64_t id);

		uint64_t CreateEntity();

	private:
		CLevel();
		~CLevel();
		static CLevel* g_pLevel;

		CTArray<CBaseEntity> m_Ents;
		CTArray<CBaseComponent*> m_Comps;

		CAssetRegistry m_AssetRegistry;
	};

	static CLevel* GetLevel() { return CLevel::GetLevel(); }
}