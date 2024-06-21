#pragma once
#include "LinkedList.h"

namespace MarkTech
{
	template<typename KeyType, typename ValType>
	class Dictionary
	{
	public:
		Dictionary();
		~Dictionary();

		void Insert(KeyType key, ValType value);
		void Remove(KeyType key);
		void Modify(KeyType key, ValType value);
		ValType Get(KeyType key);
		bool Has(KeyType key);

	private:
		struct LinkType
		{
			KeyType key;
			ValType value;
		};

		LinkedList<LinkType> m_ElementList;
	};

	template<typename KeyType, typename ValType>
	inline Dictionary<KeyType, ValType>::Dictionary()
	{
	}

	template<typename KeyType, typename ValType>
	inline Dictionary<KeyType, ValType>::~Dictionary()
	{
	}

	template<typename KeyType, typename ValType>
	inline void Dictionary<KeyType, ValType>::Insert(KeyType key, ValType value)
	{
		LinkType node = {};
		node.key = key;
		node.value = value;
		m_ElementList.Insert(node);
	}

	template<typename KeyType, typename ValType>
	inline void Dictionary<KeyType, ValType>::Remove(KeyType key)
	{
		U32 index = 0;
		for (auto it = m_ElementList.Begin(); it != nullptr; it++)
		{
			if (it->data.key == key)
				m_ElementList.Remove(index);

			index++;
		}
	}

	template<typename KeyType, typename ValType>
	inline void Dictionary<KeyType, ValType>::Modify(KeyType key, ValType value)
	{
		for (auto it = m_ElementList.Begin(); it != nullptr; it++)
		{
			if (it->data.key == key)
			{
				it->data.value = value;
			}
		}
	}

	template<typename KeyType, typename ValType>
	inline ValType Dictionary<KeyType, ValType>::Get(KeyType key)
	{
		for (auto it = m_ElementList.Begin(); it != nullptr; it++)
		{
			if (it->data.key == key)
			{
				return it->data.value;
			}
		}

		return ValType();
	}

	template<typename KeyType, typename ValType>
	inline bool Dictionary<KeyType, ValType>::Has(KeyType key)
	{
		for (auto it = m_ElementList.Begin(); it != nullptr; it++)
		{
			if (it->data.key == key)
				return true;
		}

		return false;
	}
}