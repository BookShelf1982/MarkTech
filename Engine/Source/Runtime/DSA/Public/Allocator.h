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

		void* Allocate(U32 sizeInBytes);
		void Free(void* ptr);
	private:
	};
}