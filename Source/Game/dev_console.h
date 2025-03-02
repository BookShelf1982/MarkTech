#pragma once

#include "data_structures.h"

namespace MarkTech
{
#define MAX_CONVARS 64
#define MAX_CONFNS 32

	typedef void (*PFN_ConFn)();

	struct ConVar
	{
		char name[16];
		float val;
	};

	struct ConFn
	{
		char name[16];
		PFN_ConFn fn;
	};

	struct DevData
	{
		FixedArray<ConVar, MAX_CONVARS> m_conVars;
		FixedArray<ConFn, MAX_CONFNS> m_conFns;
	};
}