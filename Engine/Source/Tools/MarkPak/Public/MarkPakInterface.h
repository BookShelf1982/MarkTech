#pragma once
#include <DLLUTLS.h>

typedef void (*PFN_CompileAsset)(const char* pFilepath, const char* pOutputPath);
typedef void (*PFN_PackageCompiledAssets)(const char* pOutputPath, const char* pPackageName);
typedef void (*PFN_CompileAndPackageAssets)(const char* pInputFilepath, const char* pOutputPath, const char* pPackageName);

struct MarkPakPFNList
{
	PFN_CompileAsset pfnCompileAsset;
	PFN_PackageCompiledAssets pfnPackageCompiledAssets;
	PFN_CompileAndPackageAssets pfnCompileAndPackageAssets;
};

typedef void (*PFN_GetMarkPakFunctions)(MarkPakPFNList* pPfnList);

MT_DLLEXPORT void GetMarkPakFunctions(MarkPakPFNList* pPfnList);