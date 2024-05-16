#pragma once
#include "PrimitiveTypes.h"
#include <Allocator.h>
#include <string.h>

/*
* Linked List container
* Author: Markos
*/

namespace MarkTech
{
	template<typename T>
	struct LinkedNode
	{
		LinkedNode();
		~LinkedNode();

		LinkedNode<T>* pNextNode;
		T data;
	};

	template<typename T>
	inline LinkedNode<T>::LinkedNode()
		:pNextNode(nullptr), data()
	{
	}

	template<typename T>
	inline LinkedNode<T>::~LinkedNode()
	{
		data.~T();
	}

	template<typename LinkedList>
	class ListIterator
	{
	public:
		using ValueType = typename LinkedList::NodeType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		ListIterator(LinkedList node)
			:m_ptr(node.m_pFirstNode)
		{
		}

		void operator++()
		{
			m_ptr = m_ptr.pNextNode;
		}

	private:
		PointerType m_ptr;
	};

	template<typename T, class A = Allocator>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();
		using Iterator = ListIterator<LinkedList<T, A>>;
		using NodeType = LinkedNode<T>;

		void Insert(const T& element);
	private:
		A m_Alloc;
		LinkedNode<T>* m_pFirstNode;
		U32 m_Size;
	};

	template<typename T, class A>
	inline LinkedList<T, A>::LinkedList()
		:m_pFirstNode(nullptr), m_Size(0)
	{
	}

	template<typename T, class A>
	inline LinkedList<T, A>::~LinkedList()
	{
		LinkedNode<T>* pCurrentNode = m_pFirstNode;
		LinkedNode<T>* pNextNode = m_pFirstNode->pNextNode;

		while (pCurrentNode != nullptr)
		{
			pNextNode = pCurrentNode->pNextNode;
			pCurrentNode->~LinkedNode();
			m_Alloc.Free(pCurrentNode);
			pCurrentNode = pNextNode;
		}
	}

	template<typename T, class A>
	inline void LinkedList<T, A>::Insert(const T& element)
	{
		LinkedNode<T>* pNode = m_pFirstNode;
		LinkedNode<T>* pPrevNode = nullptr;
		
		while (pNode != nullptr)
		{
			pPrevNode = pNode;
			pNode = pNode->pNextNode;
		}

		pNode = (LinkedNode<T>*)m_Alloc.Allocate((U32)sizeof(LinkedNode<T>));
		LinkedNode<T> temp;
		temp.data = element;
		memcpy(pNode, &temp, sizeof(temp));

		if (!m_pFirstNode)
			m_pFirstNode = pNode;
		else
			pPrevNode->pNextNode = pNode;
	}
}