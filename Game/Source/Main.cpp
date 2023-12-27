#ifdef DEBUG
#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
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

bool bClosing = false;

extern "C"
{
	void StopGameLoop() 
	{
		bClosing = true;
	}
}

HMODULE* LoadMarkTechModules( size_t* nSize )
{
	mINI::INIFile file( "Bin/Modules/Modules.ini" );
	mINI::INIStructure ini;
	if (!file.read(ini))
		return NULL;

	HMODULE* pModules = new HMODULE[ini["Modules"].size()];
	*nSize = ini["Modules"].size();
	for ( size_t i = 0; i < *nSize; i++ )
	{
		char path[256];
		sprintf_s( path, "LoadedModuleList[%zu]", i );
		char str[256];
		char* modName = (char*)ini["Modules"][path].c_str();
		sprintf_s( str, "Bin/Modules/%s.dll", modName );
		pModules[i] = LoadLibraryA( str );
	}

	return pModules;
}

pfnGetInterface* GetPfnFuncs( HMODULE* pModules, size_t nSize )
{
	pfnGetInterface* pfns = new pfnGetInterface[nSize];
	for ( size_t i = 0; i < nSize; i++ )
	{
		pfns[i] = (pfnGetInterface)GetProcAddress( pModules[i], "GetInterface" );
		if (pfns[i] == NULL)
		{
			return NULL;
		}
	}
	return pfns;
}

// Get Modules using GetInterface pfns
IModuleInterface** GetModuleInterface(pfnGetInterface* pGetInterfaces, size_t nSize)
{
	IModuleInterface** pModuleInterface = new IModuleInterface * [nSize];
	for (size_t i = 0; i < nSize; i++)
	{
		pGetInterfaces[i](&pModuleInterface[i]);
	}
	return pModuleInterface;
}

// Frees and deletes a heap allocated array of HMODULES
void FreeLibraries( HMODULE* pModules, size_t nSize )
{
	for ( size_t i = 0; i < nSize; i++ )
	{
		FreeLibrary( pModules[i] );
	}
	delete[] pModules;
}

// Preinitializes all modules in module array
void PreInitModules(IModuleInterface** pModules, size_t nSize, HINSTANCE hInstance)
{
	for (size_t i = 0; i < nSize; i++)
	{
		pModules[i]->PreInit(hInstance, StopGameLoop);
	}
}

// Initializes all modules in module array
void InitModules( IModuleInterface** pModules, size_t nSize )
{
	for ( size_t i = 0; i < nSize; i++ )
	{
		pModules[i]->Init();
	}
}

// Update all modules in module array
void UpdateModules( IModuleInterface** pModules, size_t nSize )
{
	for (size_t i = 0; i < nSize; i++)
	{
		pModules[i]->Update();
	}
}

// Shutdowns all modules in module array
void ShutdownModules( IModuleInterface** pModules, size_t nSize )
{
	for ( size_t i = 0; i < nSize; i++ )
	{
		pModules[i]->Shutdown();
	}
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
	size_t nModules = 0;
	HMODULE* pModules = LoadMarkTechModules(&nModules);
	if ( pModules == NULL )
		return 1;

	pfnGetInterface* GetInterfaces = GetPfnFuncs(pModules, nModules);
	if (GetInterfaces == NULL)
		return 1;
	IModuleInterface** pIModules = GetModuleInterface( GetInterfaces, nModules );

	PreInitModules(pIModules, nModules, hInstance);
	InitModules(pIModules, nModules);
	
	while (!bClosing)
	{
		UpdateModules(pIModules, nModules);
	}

	ShutdownModules(pIModules, nModules);
	FreeLibraries( pModules, nModules );
	delete[] GetInterfaces;
	delete[] pIModules;
	_CrtDumpMemoryLeaks();
	return 0;
}