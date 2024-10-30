/*#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef DEBUG
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
#include <Thread.h>
#include <ThreadSync.h>

#include <Dictionary.h>
#include <LinkedList.h>
#include <Array.h>
#include <MemoryAllocator.h>
#include <NBT.h>

bool gIsRunning = true;
MarkTech::Mutex gIsRunningMutex = {};
const MarkTech::U32 tickRate = 15;
static MarkTech::GameWorld* gpGameWorld = nullptr;

using namespace MarkTech;

void WindowEventHandler(void* pEvent)
{
	WindowEventType event = (WindowEventType)*(WindowEventType*)pEvent;
	switch (event)
	{
	case WINDOW_EVENT_CLOSE: 
	{
		LockMutex(&gIsRunningMutex);
		gIsRunning = false; 
		UnlockMutex(&gIsRunningMutex);
	} return;
	case WINDOW_EVENT_KEYCHANGED: 
	{ 
		WindowEventKeyChangedInfo* pKeyChanged = (WindowEventKeyChangedInfo*)pEvent;
		UpdateKeyboardState(ConvertWin32KeycodeToMarkTechKeycode(pKeyChanged->keycode), pKeyChanged->keydown); 
	} return;
	case WINDOW_EVENT_MOUSEPOS: 
	{
		WindowEventMouseMoveInfo* pMouseMoved = (WindowEventMouseMoveInfo*)pEvent;
		UpdateMousePos((I32)pMouseMoved->x, (I32)pMouseMoved->y);
	} return;
	case WINDOW_EVENT_MOUSEBUTTON_UP: 
	{
		WindowEventMouseButtonsInfo* pMouseButtons = (WindowEventMouseButtonsInfo*)pEvent;
		UpdateMouseButtons(pMouseButtons->buttons, false);
	} return;
	case WINDOW_EVENT_MOUSEBUTTON_DOWN: 
	{ 
		WindowEventMouseButtonsInfo* pMouseButtons = (WindowEventMouseButtonsInfo*)pEvent;
		UpdateMouseButtons(pMouseButtons->buttons, true);
	} return;
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

I32 TickLoop()
{
	while (true)
	{
		LockMutex(&gIsRunningMutex);
		const bool isRunning = gIsRunning;
		UnlockMutex(&gIsRunningMutex);
		if (!isRunning)
			break;

		U64 startTime = GetCurrentTimestamp();

		TickGameWorld(*gpGameWorld);

		U64 endTime = GetCurrentTimestamp();

		U64 timeDiff = endTime - startTime;
		U64 timeDuration = (U64)((F64)timeDiff * 1000.0 / (F64)GetCPUFrequency());
		if (timeDuration < tickRate)
			PutThreadToSleep((U32)(tickRate - timeDuration));
	}

	return 0;
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

	// Logging
	if (!InitLog(16 * KILOBYTE))
		return 1;

	ConsoleLog("Log initialized!");

	// Input System
	if (!InitInputSystem())
		return 1;

	ConsoleLog("Input initialized!");

	// Window Creation
	SetWindowEventHandler(WindowEventHandler);
	Window window = MakeWindow(L"MarkTech 2024", 800, 600);
	ConsoleLog("Window has been created!");
	

	// Resource Manager Creation
	PoolAllocator resourceEntryAlloc;
	PoolAllocator packageEntryAlloc;
	PoolAllocator stringTableEntryAlloc;
	CreatePoolAllocator(resourceEntryAlloc, sizeof(ResourceEntry), 1024);
	CreatePoolAllocator(packageEntryAlloc, sizeof(PackageEntry), 128);
	CreatePoolAllocator(stringTableEntryAlloc, sizeof(StringTableEntry), 256);
	
	StackAllocator resourceDataAlloc;
	CreateStackAllocator(&resourceDataAlloc, MEGABYTE);

	ResourceManagerInfo resourceManagerInfo = {};
	resourceManagerInfo.pEntryAllocator = &resourceEntryAlloc;
	resourceManagerInfo.pPackageEntryAllocator = &packageEntryAlloc;
	resourceManagerInfo.pResourceAllocator = &resourceDataAlloc;
	resourceManagerInfo.pStringTableEntryAllocator = &stringTableEntryAlloc;

	ResourceManager resourceManager = CreateResourceManager(&resourceManagerInfo);
	ConsoleLog("Resource Manager created!");
	LoadPackage(&resourceManager, "content01.mpk");
	U32 vertId = GetIdWithString(&resourceManager, "vert.spv");
	U32 fragId = GetIdWithString(&resourceManager, "frag.spv");
	LoadResource(&resourceManager, vertId);
	LoadResource(&resourceManager, fragId);
	ConsoleLog("Resources Loaded!");

	// GameWorld Creation
	GameWorld gameWorld;
	CreateGameWorld(gameWorld, 2048);
	gpGameWorld = &gameWorld;
	ConsoleLog("Game world created!");

	Renderer2D renderer = InitRenderer2D(&window); // Initialize the renderer
	ResourceEntry* pShaderEntry = GetResourceEntry(&resourceManager, vertId);
	LoadShader(&renderer, (U32*)pShaderEntry->pData, pShaderEntry->resourceSize);
	pShaderEntry = GetResourceEntry(&resourceManager, fragId);
	LoadShader(&renderer, (U32*)pShaderEntry->pData, pShaderEntry->resourceSize);
	CreatePipeline(&renderer);

	ConsoleLog("Renderer created!");

	gIsRunningMutex = MakeMutex();
	Thread tickLoop = MakeThread(TickLoop); // start simulation loop

	ConsoleLog("Loop Started!");

	// Render Loop
	while (true)
	{
		PollWindowMessages();

		LockMutex(&gIsRunningMutex);
		const bool isRunning = gIsRunning;
		UnlockMutex(&gIsRunningMutex);

		if (!isRunning)
			break;

		RenderFrame(&renderer);
	}

	ConsoleLog("Shutting down!");

	DestroyMutex(&gIsRunningMutex);
	DestroyGameWorld(gameWorld);
	ShutdownRenderer2D(&renderer);
	ShutdownResourceManager(&resourceManager);
	KillWindow(&window);
	ShutdownInputSystem();
	ShutdownLog();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}*/