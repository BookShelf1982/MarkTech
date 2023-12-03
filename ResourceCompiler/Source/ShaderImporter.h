#pragma once
#include <d3dcompiler.h>

#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

extern "C"
{
	__declspec(dllexport) int LoadShader(const char* filepath, const char* output, const char* shadertype, const char* entrypoint);
}