#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	class Log
	{
	public:
		Log();
		~Log();

		static void PrintF(const char* msg, ...);
	private:

		static Log* m_gpLog;
	};
}