#pragma once
#include "DebugStringManager.h"

namespace MarkTech
{
	class String
	{
	public:
		String();
		~String();

		void operator=(const char* string);
		const char* c_str() { return m_pString; }
	private:
		I8* m_pString;
		U64 m_Length;
	};
}