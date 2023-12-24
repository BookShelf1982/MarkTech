#pragma once
#include "DllExport.h"
#include "Core.h"
#include "Level.h"
#include "Configs.h"
#include "WinWindow.h"

namespace MarkTech
{
	class CEngine
	{
	public:

		CEngine();
		~CEngine();

		void Quit();
		bool InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
		bool ReadConfigFiles();
		void StartEngineLoop();

	private:
		bool bClosing;
		bool bShowDemoWindow;
		LARGE_INTEGER nTickFrequency, nCurrentTick, nLastTick;
		int64_t nElapsedTicks;
		float flDeltaTime;
		CWinWindow* m_pMainWindow;
		CLevel* m_pLevel;
		MGameInfo* m_pGameInfo;
		MUserSettings* m_pUserSettings;
		CInput* m_pInput;
	};
}