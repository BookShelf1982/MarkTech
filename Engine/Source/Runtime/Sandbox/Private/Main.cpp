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
#include <CmdArgs.h>
#include <MarkPakInterface.h>

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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Command Line Args
	CommandLineArgs cmdArgs = CreateCommandLineArgs();

	if (ArgCompare(&cmdArgs, 1, L"-buildpackage"))
	{
		// TODO: build package using MarkPak.dll
		MarkPakPFNList pfnList = {};
		HMODULE markPak = LoadLibraryA("MarkPak.dll");
		if (markPak == nullptr)
		{
			WindowMessage(nullptr, L"MarkTech Error!", L"Failed to find MarkPak.dll in your game install!");
			FreeCommandLineArgs(&cmdArgs);
			return 1;
		}

		PFN_GetMarkPakFunctions func = (PFN_GetMarkPakFunctions)GetProcAddress(markPak, "GetMarkPakFunctions");
		func(&pfnList);

		char inputFile[MAX_PATH_LENGTH] = "";
		char outputPath[MAX_PATH_LENGTH] = "";
		char pakName[128] = "";

		for (I32 i = 1; i < cmdArgs.argCount; i++)
		{
			if (ArgCompare(&cmdArgs, i, L"-assetlist"))
			{
				i++;
				U64 charsConverted = 0;
				wcstombs_s(&charsConverted, inputFile, cmdArgs.pArgList[i], wcslen(cmdArgs.pArgList[i]));
				continue;
			}

			if (ArgCompare(&cmdArgs, i, L"-outputpath"))
			{
				i++;
				U64 charsConverted = 0;
				wcstombs_s(&charsConverted, outputPath, cmdArgs.pArgList[i], wcslen(cmdArgs.pArgList[i]));
				continue;
			}

			if (ArgCompare(&cmdArgs, i, L"-pakname"))
			{
				i++;
				U64 charsConverted = 0;
				wcstombs_s(&charsConverted, pakName, cmdArgs.pArgList[i], wcslen(cmdArgs.pArgList[i]));
				continue;
			}
		}

		if (!(strlen(inputFile) || strlen(outputPath) || strlen(pakName)))
		{
			return 1;
		}

		pfnList.pfnCompileAndPackageAssets(inputFile, outputPath, pakName);

		FreeLibrary(markPak);
		FreeCommandLineArgs(&cmdArgs);
#ifdef DEBUG
		_CrtDumpMemoryLeaks();
#endif
		return 0;
	}

	FreeCommandLineArgs(&cmdArgs);

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

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}