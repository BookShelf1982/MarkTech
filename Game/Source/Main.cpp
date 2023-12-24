#ifdef DEBUG
#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//allocations to be of _CLIENT_BLOCK type
#else
#define MYDEBUG_NEW
#endif // _DEBUG
/* MyApp.cpp
/* Compile options needed: /Zi /D_DEBUG /MLd
/* or use a
/* Default Workspace for a Console Application to
/* build a Debug version*/
#include "crtdbg.h"

#ifdef DEBUG
#define new MYDEBUG_NEW
#endif

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include <Windows.h>
#include "ModulesShared.h"
#include "ini.h"

HRESULT LoadMarkTechModules(HMODULE** pModules, size_t* nSize)
{
	if (!*pModules)
	{
		mINI::INIFile file("Bin/Modules/Modules.ini");
		mINI::INIStructure ini;
		if (!file.read(ini))
			return E_FAIL;

		*pModules = new HMODULE[ini["Modules"].size()];
		*nSize = ini["Modules"].size();
		for (size_t i = 0; i < *nSize; i++)
		{
			char path[256];
			sprintf_s(path, "LoadedModuleList[%zu]", i);
			char str[256];
			char* modName = (char*)ini["Modules"][path].c_str();
			sprintf_s(str, "Bin/Modules/%s.dll", modName);
			*pModules[i] = LoadLibraryA(str);
		}

		return S_OK;
	}

	return E_FAIL;
}

HRESULT GetPfnFuncs(pfnGetInterface** pfns, HMODULE* pModules, size_t nSize)
{
	if (!*pfns)
	{
		*pfns = new pfnGetInterface[nSize];
		for (size_t i = 0; i < nSize; i++)
		{
			*pfns[i] = (pfnGetInterface)GetProcAddress(pModules[i], "GetInterface");
		}
		return S_OK;
	}
	return E_FAIL;
}

//Frees and deletes a heap allocated array of HMODULES
void FreeLibraries(HMODULE* pModules, size_t nSize)
{
	for (size_t i = 0; i < nSize; i++)
	{
		FreeLibrary(pModules[i]);
	}
	delete[] pModules;
}

IModuleInterface** GetModuleInterface(pfnGetInterface* pGetInterfaces, size_t nSize)
{
	IModuleInterface** pModuleInterface = new IModuleInterface*[nSize];
	for (size_t i = 0; i < nSize; i++)
	{
		pGetInterfaces[i](&pModuleInterface[i]);
	}
	return pModuleInterface;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	HMODULE* pModules = NULL;
	size_t nModules = 0;
	pfnGetInterface* GetInterfaces = NULL;

	LoadMarkTechModules(&pModules, &nModules);
	GetPfnFuncs(&GetInterfaces, pModules, nModules);
	IModuleInterface** pIModules = GetModuleInterface(GetInterfaces, nModules);
	pIModules[0]->Init();
	pIModules[0]->Shutdown();
	FreeLibraries(pModules, nModules);

	delete[] GetInterfaces;
	delete[] pIModules;
	_CrtDumpMemoryLeaks();
	return 0;
}