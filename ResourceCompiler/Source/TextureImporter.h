#pragma once
#include "DirectXTex\DirectXTex.h"

extern "C"
{
	__declspec(dllexport) int LoadTexture(const char* filepath, const char* compiledname, int mips, int compression);
}