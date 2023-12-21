#pragma once
#include "Core.h"

namespace MarkTech
{
	class MARKTECH_API CBaseEntity
	{
	friend class CLevel;
	public:
		CBaseEntity();
		~CBaseEntity();

		template<class T>
		bool HasComponent()
		{
			return GetLevel()->HasComponent<T>(m_nId);
		}

		template<class T>
		void AddComponent()
		{
			GetLevel()->CreateComponent<T>(m_nId);
		}

		template<typename T>
		T* GetComponent()
		{
			return GetLevel()->GetComponentFromEntity<T>(m_nId);
		}

		uint64_t GetId() { return m_nId; }

	protected:
		uint64_t m_nId;
	};
}

MAKE_CTARRAY_COMPATIBLE(MarkTech::CBaseEntity);
MAKE_CTARRAY_COMPATIBLE(MarkTech::CBaseEntity*);