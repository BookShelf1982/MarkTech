#pragma once
#include "DllExport.h"
#include "Core.h"
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
		
		template<class T>
		bool HasComponent(uint64_t id)
		{
			for (int i = 0; i < m_Comps.GetSize(); i++)
			{
				//Is specified type
				auto comp = dynamic_cast<T*>(m_Comps[i]);
				if (comp != nullptr)
				{
					//Is owned by specified entity
					if (m_Comps[i]->GetOwnerId() == id)
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
			m_Comps.Push(comp);
		}

		CBaseEntity* GetEntityById(uint64_t id);

		uint64_t CreateEntity();

	private:
		CLevel();
		~CLevel();
		static CLevel* g_pLevel;

		CTArray<CBaseEntity> m_Ents;
		CTArray<CBaseComponent*> m_Comps;
	};

	static CLevel* GetLevel() { return CLevel::GetLevel(); }
}