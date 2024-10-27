#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	template<typename T>
	class DynamicArray
	{
	public:
		DynamicArray();
		~DynamicArray();

		void Insert(T element);
		void Remove(U32 elementIndex);
		void Empty();

		U32 Size() const { return m_Count; }
	private:
		T* m_pArray;
		U32 m_Count;
		U32 m_ReservedAmount;
	};
}