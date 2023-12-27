#pragma once
#include "Core.h"

template<class T>
class CComponentPool
{
public:
	CComponentPool();
	~CComponentPool();

	void InitComponentPool(size_t nInitialSize);
	void UpdateComponentPool(float flDeltaTime);
	void DestroyComponentPool();

	void DisableComponent(uint64_t id);
	void CreateComponent();
private:
	T* m_pComps;
	bool m_bInitialized;
	size_t m_nDisabled;
	size_t m_nEnabled;
};

template<class T>
CComponentPool<T>::CComponentPool()
	:m_pComps(nullptr), m_bInitialized(false), m_nDisabled(0), m_nEnabled(0)
{
}

template<class T>
CComponentPool<T>::~CComponentPool()
{
}

template<class T>
inline void CComponentPool<T>::InitComponentPool(size_t nInitialSize)
{
	m_pComps = new T[nInitialSize];
	m_bInitialized = true;
	m_nDisabled = nInitialSize;
	m_nEnabled = 0;
}

template<class T>
inline void CComponentPool<T>::UpdateComponentPool(float flDeltaTime)
{
	for (size_t i = 0; i < m_nEnabled; i++)
	{
		m_pComps[i].UpdateComponent(flDeltaTime);
	}
}

template<class T>
inline void CComponentPool<T>::DestroyComponentPool()
{
	if (m_bInitialized)
		delete[] m_pComps;
}

template<class T>
inline void CComponentPool<T>::DisableComponent(uint64_t id)
{
	//Find the index of the component
	size_t index = 0;
	for (size_t i = 0; i < m_nEnabled; i++)
	{
		if (m_pComps[i].GetId() == id)
		{
			index = i;
			break;
		}
		return;
	}

	// Swap it with the last active component
	// right before the inactive ones.
	T temp = m_pComps[m_nEnabled];
	m_pComps[m_nEnabled] = m_pComps[index];
	m_pComps[index] = temp;
}

template<class T>
inline void CComponentPool<T>::CreateComponent()
{
}
