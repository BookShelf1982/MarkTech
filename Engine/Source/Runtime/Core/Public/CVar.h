#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>

namespace MarkTech
{
	struct CVarF32
	{
		char name[256];
		F32 value;
	};

	F32 GetCVar(const char* pName);
	void SetCVar(const char* pName, F32 value);
	void GiveCVarArray(CVarF32* pCVarArray, U32 numOfCVars);
	void FreeCVarArray();
}