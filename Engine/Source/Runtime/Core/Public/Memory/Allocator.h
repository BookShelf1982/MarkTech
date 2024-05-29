#pragma once
#include <PrimitiveTypes.h>

/*
* Default allocator
* Author: Markos
*/

namespace MarkTech
{
	class Allocator
	{
	public:
		Allocator();
		~Allocator();

		void* Alloc(U32 sizeInBytes, U32 alignment);
		void* Allocate(U32 sizeInBytes);
		void Free(void* ptr);
	private:
	};
}