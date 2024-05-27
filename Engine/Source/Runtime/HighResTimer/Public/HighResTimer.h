#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	class HighResTimer
	{
	public:
		static I64 GetCurrentTimestamp();
	private:
		static I64 m_PerfFrequency;
	};
}