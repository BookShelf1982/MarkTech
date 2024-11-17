#include "HashMap.h"

namespace MarkTech
{
    U32 HashKey(U32 key, U32 listSize)
	{
		return key % listSize;
	}

	bool CompareKey(U32 src, U32 key)
	{
		return src == key;
	}

	U32 HashKey(const char* pKey, U32 listSize)
	{
		U32 sum = 0;
		U32 length = (U32)strlen(pKey);
		for (U32 i = 0; i < length; i++)
		{
			sum += pKey[i];
		}

		return sum % listSize;
	}

	bool CompareKey(const char* pSrc, const char* pKey)
	{
		return strcmp(pSrc, pKey) == 0;
	}
}