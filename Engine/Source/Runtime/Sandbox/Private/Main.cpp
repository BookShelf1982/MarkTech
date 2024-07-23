#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef  DEBUG
#include <crtdbg.h>
#endif

#include <MemoryArena.h>
#include <Log.h>
#include <Window.h>
#include <CVar.h>
#include <2DRenderer.h>

#include <DynamicLinker.h>

bool gIsRunning = true;

using namespace MarkTech;

void WindowEventHandler(WindowEvent event)
{
	switch (event)
	{
	case MarkTech::WindowEvent::WINDOW_CLOSE: { gIsRunning = false; } return;
	default: { } return;
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (!LinkCoreLibrary("Core.dll")) // link core library
		return 1;

	InitMemoryArena(MEGABYTE * 2);

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
		CVarF32* pCVarArray = (CVarF32*)AllocFromMemoryArena(cvarCount * sizeof(CVarF32));
		GiveCVarArray(pCVarArray, cvarCount);
	}

	// Window Creation
	SetWindowEventHandler(WindowEventHandler);
	Window window = MakeWindow(L"MarkTech 2024", 800, 600);

	InitRenderer2D(); // Initialize the renderer

	// Game Loop
	while (gIsRunning)
	{
		PollWindowMessages();
		if (!gIsRunning)
			break;
	}

	ShutdownRenderer2D();
	KillWindow(&window);
	KillMemoryArena();

	UnlinkCoreLibrary(); // unlink core library

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}