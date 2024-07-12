#include "Random\XorShift.h"

namespace MarkTech
{
	U32 XorShift32(U32 number)
	{
		number ^= number << 13;
		number ^= number >> 17;
		number ^= number << 5;

		return number;
	}

	U64 XorShift64(U64 number)
	{
		number ^= number << 13;
		number ^= number >> 7;
		number ^= number << 17;

		return number;
	}
}

