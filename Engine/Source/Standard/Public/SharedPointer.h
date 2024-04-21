#pragma once
#include <vector>
#include <memory>

template<typename T>
class CRefCounted
{
public:
	CRefCounted(T* ptr);
	~CRefCounted();

	void AddRef();
	void RemoveRef();

	T* GetPtr() { return m_ptr; }
	uint32_t GetRefCount() { return nCount; }

private:
	uint32_t nCount;
	T* m_ptr;
};

template<typename T>
inline CRefCounted<T>::CRefCounted(T* ptr)
	:m_ptr(nullptr), nCount(0)
{
	m_ptr = ptr;
}

template<typename T>
inline CRefCounted<T>::~CRefCounted()
{
	delete m_ptr;
}

template<typename T>
inline void CRefCounted<T>::AddRef()
{
	nCount++;
}

template<typename T>
inline void CRefCounted<T>::RemoveRef()
{
	nCount--;
	if (nCount == 0)
	{
		delete this;
	}
}

template<typename T>
class CTSharedPointer
{
public:
	CTSharedPointer();
	CTSharedPointer(T* ptr);
	CTSharedPointer(const CTSharedPointer& other);
	~CTSharedPointer();

	T* GetAddress() { return m_pRef->GetPtr(); }

	void Reset();

	T* operator->();
	void operator=(const CTSharedPointer& other);
	void operator=(T* ptr);

private:
	CRefCounted<T>* m_pRef;
};

template<typename T>
inline CTSharedPointer<T>::CTSharedPointer()
	:m_pRef(nullptr)
{
}

template<typename T>
inline CTSharedPointer<T>::CTSharedPointer(T* ptr)
	:m_pRef(nullptr)
{
	m_pRef = new CRefCounted<T>(ptr);
	m_pRef->AddRef();
}

template<typename T>
inline CTSharedPointer<T>::CTSharedPointer(const CTSharedPointer& other)
	:m_pRef(nullptr)
{
	m_pRef = other.m_pRef;
	m_pRef->AddRef();
}

template<typename T>
inline CTSharedPointer<T>::~CTSharedPointer()
{
	if(m_pRef)
		m_pRef->RemoveRef();
}

template<typename T>
inline void CTSharedPointer<T>::Reset()
{
	if (m_pRef)
	{
		m_pRef->RemoveRef();
		m_pRef = nullptr;
	}
}

template<typename T>
inline void CTSharedPointer<T>::operator=(const CTSharedPointer& other)
{
	if (m_pRef)
		m_pRef->RemoveRef();

	m_pRef = other.m_pRef;
	m_pRef->AddRef();
}

template<typename T>
inline void CTSharedPointer<T>::operator=(T* ptr)
{
	if (!m_pRef)
	{
		m_pRef = new CRefCounted<T>(ptr);
		m_pRef->AddRef();
	}
}

template<typename T>
inline T* CTSharedPointer<T>::operator->()
{
	return m_pRef->GetPtr();
}
