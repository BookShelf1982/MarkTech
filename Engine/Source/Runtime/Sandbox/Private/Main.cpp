#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef  DEBUG
#include <crtdbg.h>
#endif

#include <File.h>

#include <Log.h>
#include <Window.h>
#include <CVar.h>
#include <2DRenderer.h>
#include <ResourceManager.h>
#include <StackAllocator.h>
#include <HighResTimer.h>
#include <GameWorld.h>
#include <Input.h>
#include <Math3D.h>

bool gIsRunning = true;

using namespace MarkTech;

void WindowEventHandler(WindowEvent event, U64 param, U64 param2)
{
	switch (event)
	{
	case WindowEvent::WINDOW_CLOSE: { gIsRunning = false; } return;
	case WindowEvent::WINDOW_KEYCHANGED: { UpdateKeyboardState(ConvertWin32KeycodeToMarkTechKeycode(param), param2); } return;
	case WindowEvent::WNIDOW_MOUSEPOS: { UpdateMousePos((I32)param, (I32)param2); } return;
	case WindowEvent::WINDOW_MOUSEBUTTON: { UpdateMouseButtons((U8)param, param2); } return;
	default: { } return;
	}
}

#ifdef MT_PLATFORM_WINDOWS
HMODULE fileMod = NULL;
#endif

PFN_FindAllFilesInPath FindAllFilesInPath = nullptr;
PFN_FileListFree FileListFree = nullptr;
PFN_GetExtension GetExtension = nullptr;
PFN_GetFilename GetFilename = nullptr;
PFN_AddExtension AddExtension = nullptr;
PFN_AddFilename AddFilename = nullptr;
PFN_ChangeExtension ChangeExtension = nullptr;

PFN_FOpen FOpen = nullptr;
PFN_FClose FClose = nullptr;
PFN_FRead FRead = nullptr;
PFN_FWrite FWrite = nullptr;
PFN_FSeek FSeek = nullptr;

void LinkFileSystem()
{
#ifdef MT_PLATFORM_WINDOWS
	fileMod = LoadLibraryA("FileSystem.dll");
	if (fileMod == NULL)
		return;

	FOpen = (PFN_FOpen)GetProcAddress(fileMod, "FOpen");
	FClose = (PFN_FClose)GetProcAddress(fileMod, "FClose");
	FWrite = (PFN_FWrite)GetProcAddress(fileMod, "FWrite");
	FRead = (PFN_FRead)GetProcAddress(fileMod, "FRead");
	FSeek = (PFN_FSeek)GetProcAddress(fileMod, "FSeek");
	AddFilename = (PFN_AddFilename)GetProcAddress(fileMod, "AddFilename");
	AddExtension = (PFN_AddExtension)GetProcAddress(fileMod, "AddExtension");
	GetExtension = (PFN_GetExtension)GetProcAddress(fileMod, "GetExtension");
	GetFilename = (PFN_GetFilename)GetProcAddress(fileMod, "GetFilename");
	FindAllFilesInPath = (PFN_FindAllFilesInPath)GetProcAddress(fileMod, "FindAllFilesInPath");
	FileListFree = (PFN_FileListFree)GetProcAddress(fileMod, "FileListFree");
	ChangeExtension = (PFN_ChangeExtension)GetProcAddress(fileMod, "ChangeExtension");
#endif
}

void UnlinkFileSystem()
{
#ifdef MT_PLATFORM_WINDOWS
	FreeLibrary(fileMod);
#endif
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	LinkFileSystem();

	// Command Line Args
	bool createCVarSystem = false;

	LPWSTR* pArglist; 
	I32 argCount;
	pArglist = CommandLineToArgvW(GetCommandLineW(), &argCount);

	for (I32 i = 0; i < argCount; i++)
	{
		if (wcscmp(pArglist[i], L"-console") == 0)
			createCVarSystem = true;
	}

	LocalFree(pArglist);

	// CVar Creation
	if (createCVarSystem)
	{
		const U32 cvarCount = 1024;
		CVarF32* pCVarArray = (CVarF32*)malloc(cvarCount * sizeof(CVarF32));
		GiveCVarArray(pCVarArray, cvarCount);
	}

	// Input System
	InitInputSystem();

	// Window Creation
	SetWindowEventHandler(WindowEventHandler);
	Window window = MakeWindow(L"MarkTech 2024", 800, 600);
	
	// Resource Manager Creation
	PoolAllocator resourceEntryAlloc = CreatePoolAllocator(sizeof(ResourceEntry), 1024);
	PoolAllocator packageEntryAlloc = CreatePoolAllocator(sizeof(PackageEntry), 128);
	StackAllocator resourceDataAlloc = CreateStackAllocator(MEGABYTE);

	InitResourceManager(&resourceEntryAlloc, &packageEntryAlloc, &resourceDataAlloc);

	// GameWorld Creation
	GameWorld gameWorld = CreateGameWorld(2048);

	InitRenderer2D(); // Initialize the renderer

	// Game Loop
	while (gIsRunning)
	{
		PollWindowMessages();
		if (!gIsRunning)
			break;
	}

	DestroyGameWorld(&gameWorld);
	ShutdownRenderer2D();
	ShutdownResourceManager();
	KillWindow(&window);
	ShutdownInputSystem();
	FreeCVarArray();

	UnlinkFileSystem();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}