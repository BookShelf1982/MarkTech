#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	class String
	{
	public:
		String();
		String(const char* str);
		~String();

		void Format(const char* format, ...);

		void operator=(const char* str);
		const char* c_str() const { return m_pString; }
	private:
		I8* m_pString;
		U64 m_Length;
	};
}