#pragma once
#ifdef _MSC_VER
#define EXPORT_DLL extern "C" __declspec(dllexport)
#else 
#dfine EXPORT_DLL
#endif