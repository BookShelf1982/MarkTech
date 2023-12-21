#pragma once
#include "Core.h"
namespace MarkTech
{
#define MAX_COMPONENTS 16384 

	template<class T>
	class MARKTECH_API CComponentPool
	{
	public:
		CComponentPool();
		~CComponentPool();

		void InitalizePool();
		void UpdateComponents(float flDeltaTime);
		bool ComponentExists(uint64_t id);
		void AddComp(T id);
		void DeleteComponent(uint64_t id);
		T* GetComponentFromId(uint64_t id);

		size_t GetEnabledSize() { return m_nEnabledComps; }
		size_t GetDisabledSize() { return m_nDisabledComps; }
	private:
		T* m_pComponents;
		size_t m_nEnabledComps;
		size_t m_nDisabledComps;
		bool m_bInitialized;
	};

	template<class T>
	inline CComponentPool<T>::CComponentPool()
		: m_nDisabledComps(0), m_nEnabledComps(0), m_bInitialized(false), m_pComponents(nullptr)
	{
	}

	template<class T>
	inline CComponentPool<T>::~CComponentPool()
	{
		if(m_bInitialized)
			delete[] m_pComponents;
	}

	template<class T>
	inline void CComponentPool<T>::InitalizePool()
	{
		m_bInitialized = true;
		m_pComponents = new T[MAX_COMPONENTS];
		m_nDisabledComps = MAX_COMPONENTS;
	}

	template<class T>
	inline void CComponentPool<T>::UpdateComponents(float flDeltaTime)
	{
		for (size_t i = 0; i < m_nEnabledComps; i++)
		{
			m_pComponents[i].UpdateComponent(flDeltaTime);
		}
	}

	template<class T>
	inline bool CComponentPool<T>::ComponentExists(uint64_t id)
	{
		for (size_t i = 0; i < m_nEnabledComps; i++)
		{
			if (m_pComponents[i].GetId() == id)
				return true;
		}
		return false;
	}

	template<class T>
	inline void CComponentPool<T>::AddComp(T comp)
	{
		m_pComponents[m_nEnabledComps] = comp;
		m_nDisabledComps--;
		m_nEnabledComps++;
	}

	template<class T>
	inline void CComponentPool<T>::DeleteComponent(uint64_t id)
	{
		
	}

	template<class T>
	inline T* CComponentPool<T>::GetComponentFromId(uint64_t id)
	{
		for (size_t i = 0; i < m_nEnabledComps; i++)
		{
			if (m_pComponents[i].GetId() == id)
			{
				return &m_pComponents[i];
			}
		}
		return 0;
	}
}