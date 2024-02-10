#pragma once

#include <shaderc\shaderc.hpp>

extern "C"
{
	__declspec(dllexport) int LoadShader(const char* filepath, const char* output, const char* entrypoint);
}