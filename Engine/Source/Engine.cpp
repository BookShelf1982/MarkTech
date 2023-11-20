#include "Engine.h"
#include "TransformComponent.h"

namespace MarkTech
{
	CEngine* CEngine::g_pEngine = new CEngine();

	CEngine::CEngine()
	{
		bClosing = false;
		Window = new CWinWindow();
	}

	CEngine::~CEngine()
	{
		DestroyWindow(Window->GetHWND());
		delete Window;
		CD3D11Renderer::GetD3DRenderer()->DestroyRenderer();
		MGameInfo::GetGameInfo()->Destroy();
		MUserSettings::GetUserSettings()->Destroy();
		CLevel::GetLevel()->DestroyLevel();
	}

	bool CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
	{
		//Read values from GameInfo.ini file
		if (!ReadConfigFiles())
		{
			return false;
		}

		//Create Window
		Window->CreateWinWindow(
			L"WinWindow",								//Class Name
			L"jknds",									//Window Title
			CW_USEDEFAULT,								//X Pos
			CW_USEDEFAULT,								//Y Pos
			MUserSettings::GetUserSettings()->nVSWidth,										//Configured Width
			MUserSettings::GetUserSettings()->nVSHeight,										//Configured Length
			hInstance,									//hImstance
			nCmdShow);									//nCmdShow

		if (!GetLevel()->InitLevel())	//Initialize Level
		{
			return false;
		}

		if (!CD3D11Renderer::GetD3DRenderer()->InitRenderer(Window->GetHWND()))
		{
			return false;
		}

		uint64_t ent1Id = GetLevel()->CreateEntity();
		CBaseEntity* ent1 = GetLevel()->GetEntityById(ent1Id);
		//ent1->AddComponent<CTransformComponent>();

		bool boolean = ent1->HasComponent<CTransformComponent>();

		return true;
	}

	bool CEngine::ReadConfigFiles()
	{
		//Read alues from GameInfo.ini file
		if (!MGameInfo::GetGameInfo()->Init())
		{
			Window->CreateErrorBox(L"Unable to find GameInfo.ini");
			return false;
		}

		if (!MUserSettings::GetUserSettings()->Init())
		{
			Window->CreateErrorBox(L"Unable to find or generate UserSettings.ini");
			return false;
		}
		return true;
	}

	void CEngine::StartEngineLoop()
	{
		while (!bClosing)
		{
			flDeltaTime = (float)Window->nElapsedTicks / (float)Window->nTickFrequency.QuadPart;

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
				}
				if (bClosing)
				{
					break;
				}
			}

			GetLevel()->UpdateLevel(flDeltaTime);

			CD3D11Renderer::GetD3DRenderer()->RenderFrame(Window->GetHWND());

			if (CInput::GetInput()->IsKeyDown(MTVK_Escape))
			{
				Quit();
			}
		}
	}

	void CEngine::DestroyEngine()
	{
		delete g_pEngine;
	}

	void CEngine::Quit()
	{
		bClosing = true;
	}
}

int LaunchEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (!MarkTech::CEngine::GetEngine()->InitEngine(hInstance, pCmdLine, nCmdShow))
	{
		return 1;
	}
	MarkTech::CEngine::GetEngine()->StartEngineLoop();
	MarkTech::CEngine::GetEngine()->DestroyEngine();
	return 0;
}