#include "Engine.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace MarkTech
{
	CEngine* CEngine::g_pEngine = new CEngine();

	CEngine::CEngine()
	{
		_CrtCheckMemory();
		bClosing = false;
		m_pMainWindow = new CWinWindow();
	}

	CEngine::~CEngine()
	{
		CD3D11Renderer::GetD3DRenderer()->DestroyRenderer();
		MGameInfo::GetGameInfo()->Destroy();
		MUserSettings::GetUserSettings()->Destroy();
		CLevel::GetLevel()->DestroyLevel();
		delete m_pMainWindow;
	}

	//Engine init func
	bool CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
	{
		//Read values from GameInfo.ini file
		if (!ReadConfigFiles())
		{
			return false;
		}

		//Create Window
		m_pMainWindow->CreateWinWindow(
			L"WinWindow",									//Class Name
			L"jknds",										//Window Title
			CW_USEDEFAULT,									//X Pos
			CW_USEDEFAULT,									//Y Pos
			MUserSettings::GetUserSettings()->nVSWidth,		//Configured Width
			MUserSettings::GetUserSettings()->nVSHeight,	//Configured Length
			hInstance,										//hImstance
			nCmdShow);										//nCmdShow

		if (!GetLevel()->InitLevel())	//Initialize Level
		{
			return false;
		}

		if (!CD3D11Renderer::GetD3DRenderer()->InitRenderer(*m_pMainWindow))
		{
			return false;
		}

		bIsEditor = false;

		uint64_t entid = GetLevel()->CreateEntity();
		GetLevel()->CreateComponent<CTransformComponent>(entid);
		GetLevel()->CreateComponent<CCameraComponent>(entid);

		/*uint64_t mdlentid = GetLevel()->CreateEntity();
		GetLevel()->CreateComponent<CTransformComponent>(mdlentid);
		GetLevel()->CreateComponent<CModelComponent>(mdlentid);

		GetLevel()->LoadAsset("output.mmdl", MModel);*/

		QueryPerformanceCounter(&nLastTick);
		QueryPerformanceFrequency(&nTickFrequency);

		return true;
	}

	//editor init func
	bool CEngine::InitEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
	{
		//Read values from GameInfo.ini file
		if (!ReadConfigFiles())
		{
			return false;
		}

		//Create Window
		m_pMainWindow->CreateWinWindow(
			L"WinWindow",									//Class Name
			L"jknds",										//Window Title
			CW_USEDEFAULT,									//X Pos
			CW_USEDEFAULT,									//Y Pos
			MUserSettings::GetUserSettings()->nVSWidth,		//Configured Width
			MUserSettings::GetUserSettings()->nVSHeight,	//Configured Length
			hInstance,										//hImstance
			nCmdShow);										//nCmdShow

		if (!GetLevel()->InitLevel())	//Initialize Level
		{
			return false;
		}

		if (!CD3D11Renderer::GetD3DRenderer()->InitRenderer(*m_pMainWindow))
		{
			return false;
		}

		bIsEditor = true;

		QueryPerformanceCounter(&nLastTick);
		QueryPerformanceFrequency(&nTickFrequency);

		return true;
	}

	bool CEngine::ReadConfigFiles()
	{
		//Read alues from GameInfo.ini file
		if (!MGameInfo::GetGameInfo()->Init())
		{
			m_pMainWindow->CreateErrorBox(L"Unable to find GameInfo.ini");
			return false;
		}

		if (!MUserSettings::GetUserSettings()->Init())
		{
			m_pMainWindow->CreateErrorBox(L"Unable to find or generate UserSettings.ini");
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
				m_pMainWindow->MessageLoop(msg);
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

			if (CInput::GetInput()->IsKeyDown(MTVK_Escape))
			{
				Quit();
			}

			GetLevel()->UpdateLevel(flDeltaTime);

			CD3D11Renderer::GetD3DRenderer()->UpdateRender(*m_pMainWindow);

			CD3D11Renderer::GetD3DRenderer()->RenderFrame(*m_pMainWindow);
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
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}

int LaunchEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (!MarkTech::CEngine::GetEngine()->InitEditor(hInstance, pCmdLine, nCmdShow))
	{
		return 1;
	}
	MarkTech::CEngine::GetEngine()->StartEngineLoop();
	MarkTech::CEngine::GetEngine()->DestroyEngine();
	return 0;
}
