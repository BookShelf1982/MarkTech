#pragma once
#include <PrimitiveTypes.h>
#include <PoolAllocator.h>

namespace MarkTech
{
	template<typename Key, typename Val>
	struct Dictionary
	{
		struct KeyValNode
		{
			Key key;
			Val value;
			KeyValNode* pNext;
		};

		KeyValNode* pStart;
		PoolAllocator nodePool;
	};

	template<typename Key, typename Val>
	Dictionary<Key, Val> CreateDictionary();
	
	template<typename Key, typename Val>
	void DestroyDictionary(Dictionary<Key, Val>* pDictionary);

	template<typename Key, typename Val>
	void InsertDictionary(Dictionary<Key, Val>* pDictionary, Key key, Val value);
	
	template<typename Key, typename Val>
	void Remove(Dictionary<Key, Val>* pDictionary, Key key);
	
	template<typename Key, typename Val>
	Dictionary<Key, Val> CreateDictionary()
	{
		Dictionary<Key, Val> dict = {};
		dict.nodePool = CreatePoolAllocator(sizeof(Dictionary<Key, Val>::KeyValNode), 64);
		dict.pStart = nullptr;
		return dict;
	}

	template<typename Key, typename Val>
	void DestroyDictionary(Dictionary<Key, Val>* pDictionary)
	{
		FreePoolAllocator(&pDictionary->nodePool);
	}

	template<typename Key, typename Val>
	void InsertDictionary(Dictionary<Key, Val>* pDictionary, Key key, Val value)
	{
		Dictionary<Key, Val>::KeyValNode* pNode = (Dictionary<Key, Val>::KeyValNode*)AllocFromPool(&pDictionary->nodePool);
		pNode->key = key;
		pNode->value = value;
		pNode->pNext = nullptr;

		if (pDictionary->pStart)
		{
			Dictionary<Key, Val>::KeyValNode* pLastNode = pDictionary->pStart;
			while (pLastNode->pNext != nullptr)
			{
				pLastNode = pLastNode->pNext;
			}

			pLastNode->pNext = pNode;
			return;
		}

		pDictionary->pStart = pNode;
	}

	template<typename Key, typename Val>
	void Remove(Dictionary<Key, Val>* pDictionary, Key key)
	{
	}
}