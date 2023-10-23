#include "Engine.h"
#include "ModelEntity.h"

CEngine::CEngine()
{
	bClosing = false;
	bShowDemoWindow = true;
	Window = new CWinWindow();
	Map = new CMap();
}

CEngine::~CEngine()
{
	delete Window;
	delete Map;
}

bool CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	//Read alues from GameInfo.ini file
	if (!ReadConfigFiles())
	{
		return false;
	}

	//Convert string to wstring
	std::wstring WindowName(MGameInfo::GetGameInfo().GameName.begin(), MGameInfo::GetGameInfo().GameName.end());

	//If Window name is nothing then default to MarkTech 2023
	if (WindowName == L"") 
	{ 
		WindowName = L"MarkTech 2023"; 
	}

	//Create Window
	Window->CreateWinWindow(
		L"WinWindow",								//Class Name
		WindowName.c_str(),							//Window Title
		CW_USEDEFAULT,								//X Pos
		CW_USEDEFAULT,								//Y Pos
		MUserSettings::GetUserSettings().nVSWidth,	//Configured Width
		MUserSettings::GetUserSettings().nVSLength, //Configured Length
		hInstance,									//hImstance
		nCmdShow);									//nCmdShow

	CD3DRenderer::GetD3DRenderer()->InitRenderer(Window->GetHWND());

	//CModelEntity* ent = new CModelEntity("James");

	//Map->SpawnEntity(ent, MTransform());


	QueryPerformanceCounter(&nLastTick);
	QueryPerformanceFrequency(&nTickFrequency);

	return true;
}

bool CEngine::InitEngineEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{


	return true;
}

bool CEngine::ReadConfigFiles()
{
	//Read alues from GameInfo.ini file
	if (!MGameInfo::GetGameInfo().Init())
	{
		Window->CreateErrorBox(L"Unable to find GameInfo.ini");
		return false;
	}

	if (!MUserSettings::GetUserSettings().Init())
	{
		Window->CreateErrorBox(L"Unable to find UserSettings.ini");
		return false;
	}
	return true;
}

void CEngine::StartEngineLoop()
{
	while (!bClosing)
	{
		QueryPerformanceCounter(&nCurrentTick);
		nElapsedTicks = nCurrentTick.QuadPart - nLastTick.QuadPart;

		flDeltaTime = (float)nElapsedTicks / (float)nTickFrequency.QuadPart;

		nLastTick = nCurrentTick;

		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			Window->MessageLoop(msg);
			switch (msg.message)
			{
				case WM_QUIT:
				{
					bClosing = true;
				}break;

				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:
				{
					uint32_t keycode = msg.wParam;

					bool bWasDown = (msg.lParam & (1 << 30)) != 0;
					bool bIsDown = (msg.lParam & (1 << 31)) == 0;

					CInput::GetInput()->PollInput(keycode, bIsDown, bWasDown);
				}break;
			}
			if (bClosing)
			{
				break;
			}
		}

		Map->UpdateEntities(flDeltaTime);

		CD3DRenderer::GetD3DRenderer()->RenderFrame(Window->GetHWND());

		if (CInput::GetInput()->IsKeyDown(MTVK_Escape))
		{
			DestroyWindow(Window->GetHWND());
		}
	}
}