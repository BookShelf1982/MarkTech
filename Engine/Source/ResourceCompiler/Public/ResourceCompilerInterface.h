#pragma once
#include "Core.h"

extern "C"
{
	DLLEXPORT void InitModule();
	DLLEXPORT void UpdateModule();
	DLLEXPORT void ShutdownModule();
}