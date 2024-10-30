#pragma once
#include <PrimitiveTypes.h>
#include "PoolAllocator.h"
#include <stdlib.h>

namespace MarkTech
{
	template<typename T>
	struct LinkedList
	{
		struct NodeType
		{
			T data;
			NodeType* pNext;
		};

		NodeType* pStart;
		NodeType* pEnd;
		PoolAllocator* pPool;
	};

	template<typename T>
	void CreateLinkedList(LinkedList<T>& list, PoolAllocator* pPool)
	{
		list.pStart = nullptr;
		list.pEnd = nullptr;
		list.pPool = pPool;
	}

	template<typename T>
	void InsertLinkedListItem(LinkedList<T>& list, T item)
	{
		LinkedList<T>::NodeType* pNode = nullptr;
		if (list.pPool)
			pNode = (LinkedList<T>::NodeType*)AllocFromPool(*list.pPool);
		else
			pNode = (LinkedList<T>::NodeType*)malloc(sizeof(LinkedList<T>::NodeType));

		pNode->data = item;
		pNode->pNext = nullptr;

		if (!list.pStart)
		{
			list.pStart = pNode;
			list.pEnd = pNode;
		}
		else
		{
			LinkedList<T>::NodeType* pLast = list.pStart;
			while (pLast->pNext != nullptr)
			{
				pLast = pLast->pNext;
			}

			pLast->pNext = pNode;
			list.pEnd = pNode;
		}
	}

	template<typename T>
	void RemoveLastLinkedListItem(LinkedList<T>& list)
	{
		LinkedList<T>::NodeType* pEnd = list.pEnd;
		if (list.pStart == list.pEnd)
		{
			list.pStart = nullptr;
			list.pEnd = nullptr;
		}
		else
		{
			LinkedList<T>::NodeType* pNode = list.pStart;
			while (pNode->pNext->pNext != nullptr)
			{
				pNode = pNode->pNext;
			}
			list.pEnd = pNode;
		}

		if (list.pPool)
			FreeToPool(*list.pPool, pEnd);
		else
			free(pEnd);
	}

	template<typename T>
	void RemoveLinkedListItemWithIndex(LinkedList<T>& list, U32 index)
	{
		LinkedList<T>::NodeType* pNode = list.pStart;
		LinkedList<T>::NodeType* pPrevNode = nullptr;
		U32 currentIndex = 0;
		while (pNode != nullptr)
		{
			if (currentIndex == index)
				break;

			currentIndex++;
			pPrevNode = pNode;
			pNode = pNode->pNext;
		}
		
		if (pNode == list.pStart)
		{
			list.pStart = list.pStart->pNext;
		}
		else
		{
			pPrevNode->pNext = pNode->pNext;
		}

		if (pNode == list.pEnd)
		{
			list.pEnd = pPrevNode;
		}

		if (!list.pPool)
			free(pNode);
		else
			FreeToPool(*list.pPool, pNode);
	}

	template<typename T>
	void RemoveLinkedListItem(LinkedList<T>& list, void* pNodePtr)
	{
		LinkedList<T>::NodeType* pNode = list.pStart;
		LinkedList<T>::NodeType* pPrevNode = nullptr;

		while (pNode != (LinkedList<T>::NodeType*)pNodePtr)
		{
			pPrevNode = pNode;
			pNode = pNode->pNext;
		}

		if (pNode == list.pStart)
		{
			list.pStart = list.pStart->pNext;
		}
		else
		{
			pPrevNode->pNext = pNode->pNext;
		}

		if (pNode == list.pEnd)
		{
			list.pEnd = pPrevNode;
		}

		if (!list.pPool)
			free(pNode);
		else
			FreeToPool(*list.pPool, pNode);
	}

	template<typename T>
	void DestroyLinkedList(LinkedList<T>& list)
	{
		if (list.pStart)
		{
			if (list.pPool)
			{
				LinkedList<T>::NodeType* pNode = list.pStart;
				while (pNode->pNext != nullptr)
				{
					FreeToPool(*list.pPool, pNode);
					pNode = pNode->pNext;
				}
				return;
			}

			LinkedList<T>::NodeType* pNode = list.pStart;
			while (pNode->pNext != nullptr)
			{
				LinkedList<T>::NodeType* pNextNode = pNode->pNext;
				free(pNode);
				pNode = pNextNode;
			}
			
			free(pNode); // free last node
		}
	}
}