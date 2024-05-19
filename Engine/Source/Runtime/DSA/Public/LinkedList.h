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
	/*
	* Node type in the linked list.
	*/
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
	}

	/*
	* List Iterator iterates through a linked list
	*/
	template<typename LinkedList>
	class ListIterator
	{
	public:
		using ValueType = typename LinkedList::NodeType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		ListIterator(PointerType node)
			:m_ptr(node)
		{
		}

		void operator++(I32)
		{
			m_ptr = m_ptr->pNextNode;
		}

		bool operator==(const ListIterator& other)
		{
			return m_ptr == other.m_ptr;
		}

		bool operator!=(const ListIterator& other)
		{
			return m_ptr != other.m_ptr;
		}

		PointerType operator->()
		{
			return m_ptr;
		}
	private:
		PointerType m_ptr;
	};

	/*
	* Linked list container
	*/
	template<typename T, class A = Allocator>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();
		using Iterator = ListIterator<LinkedList<T, A>>;
		using NodeType = LinkedNode<T>;

		void Insert(const T& element);
		void Remove(U32 index);
		Iterator Begin() { return Iterator(m_pFirstNode); }
	private:
		A m_Alloc;
		LinkedNode<T>* m_pFirstNode;
		U32 m_Size;
	};

	/*
	* Sets values to 0
	*/
	template<typename T, class A>
	inline LinkedList<T, A>::LinkedList()
		:m_pFirstNode(nullptr), m_Size(0)
	{
	}

	/*
	* Destroys all nodes and frees node memory
	*/
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

	/*
	* Insert a node in linked list
	*/
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

	/*
	* Removes a node from the linked list
	*/
	template<typename T, class A>
	inline void LinkedList<T, A>::Remove(U32 index)
	{
		LinkedNode<T>* pNode = m_pFirstNode;
		LinkedNode<T>* pPrevNode = nullptr;

		for (U32 i = 0; i < index; i++)
		{
			pPrevNode = pNode;
			pNode = pNode->pNextNode;
		}
		
		if (!pNode)
			return;

		pPrevNode->pNextNode = pNode->pNextNode;
		
		pNode->~LinkedNode();
		m_Alloc.Free(pNode);
	}
}