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

void BuildPackage(const CommandLineArgs* pArgs)
{
	MarkPakPFNList pfnList = {};

#ifdef MT_PLATFORM_WINDOWS
	HMODULE markPak = LoadLibraryA("MarkPak.dll");
	if (markPak == nullptr)
	{
		WindowMessage(nullptr, L"MarkTech Error!", L"Failed to find MarkPak.dll in your game install!");
		return;
	}
#endif

	PFN_GetMarkPakFunctions func = (PFN_GetMarkPakFunctions)GetProcAddress(markPak, "GetMarkPakFunctions");
	func(&pfnList);

	char inputFile[MAX_PATH_LENGTH] = "";
	char outputPath[MAX_PATH_LENGTH] = "";
	char signature[32] = "";
	char pakName[128] = "";
	U32 flags = 0;

	for (I32 i = 1; i < pArgs->argCount; i++)
	{
		if (ArgCompare(pArgs, i, L"-assetlist"))
		{
			i++;
			U64 charsConverted = 0;
			wcstombs_s(&charsConverted, inputFile, pArgs->pArgList[i], wcslen(pArgs->pArgList[i]));
			i++;
		}

		if (ArgCompare(pArgs, i, L"-outputpath"))
		{
			i++;
			U64 charsConverted = 0;
			wcstombs_s(&charsConverted, outputPath, pArgs->pArgList[i], wcslen(pArgs->pArgList[i]));
			i++;
		}

		if (ArgCompare(pArgs, i, L"-pakname"))
		{
			i++;
			U64 charsConverted = 0;
			wcstombs_s(&charsConverted, pakName, pArgs->pArgList[i], wcslen(pArgs->pArgList[i]));
			i++;
		}

		if (ArgCompare(pArgs, i, L"-s"))
		{
			i++;
			U64 charsConverted = 0;
			wcstombs_s(&charsConverted, signature, pArgs->pArgList[i], wcslen(pArgs->pArgList[i]));
			i++;
		}
		
		if (ArgCompare(pArgs, i, L"-d"))
		{
			i++;
			U64 charsConverted = 0;
			flags = 0x01;
			i++;
		}
	}

	if (!(strlen(inputFile) || strlen(outputPath) || strlen(pakName)))
	{
		return;
	}

	pfnList.pfnCompileAndPackageAssets(inputFile, outputPath, pakName, signature, flags);

#ifdef MT_PLATFORM_WINDOWS
	FreeLibrary(markPak);
#endif
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Command Line Args
	CommandLineArgs cmdArgs = CreateCommandLineArgs();

	if (ArgCompare(&cmdArgs, 1, L"-buildpackage"))
	{
		BuildPackage(&cmdArgs);
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
	PoolAllocator stringTableEntryAlloc = CreatePoolAllocator(sizeof(StringTableEntry), 256);
	StackAllocator resourceDataAlloc = CreateStackAllocator(MEGABYTE);

	InitResourceManager(&resourceEntryAlloc, &packageEntryAlloc, &stringTableEntryAlloc, &resourceDataAlloc);
	LoadPackage("content01.mpk");
	U32 vertId = GetIdWithString("vert.spv");
	U32 fragId = GetIdWithString("frag.spv");
	U32 textId = GetIdWithString("text.txt");
	LoadResource(textId);
	LoadResource(vertId);
	LoadResource(fragId);
	ResourceEntry* pEntry = GetResourceEntry(textId);

	// GameWorld Creation
	GameWorld gameWorld = CreateGameWorld(2048);

	Renderer2D renderer = InitRenderer2D(&window); // Initialize the renderer
	ResourceEntry* pShaderEntry = GetResourceEntry(vertId);
	LoadShader(&renderer, (U32*)pShaderEntry->pData, pShaderEntry->resourceSize);
	pShaderEntry = GetResourceEntry(fragId);
	LoadShader(&renderer, (U32*)pShaderEntry->pData, pShaderEntry->resourceSize);
	CreatePipeline(&renderer);

	// Game Loop
	while (gIsRunning)
	{
		PollWindowMessages();
		if (!gIsRunning)
			break;
		RenderFrame(&renderer);
	}

	DestroyGameWorld(&gameWorld);
	ShutdownRenderer2D(&renderer);
	ShutdownResourceManager();
	KillWindow(&window);
	ShutdownInputSystem();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}