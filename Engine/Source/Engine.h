#pragma once
#include "DllExport.h"
#include "Core.h"
#include "Level.h"
#include "WinWindow.h"
#include "Configs.h"
#include "Globals.h"

namespace MarkTech
{
	class CEngine
	{
	public:

		void Quit();

		bool InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
		bool InitEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

		bool ReadConfigFiles();

		void StartEngineLoop();

		void DestroyEngine();

		static CEngine* GetEngine() { return g_pEngine; }

	private:

		static CEngine* g_pEngine;

		CEngine();
		~CEngine();

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

#ifdef MT_PLATFORM_WINDOWS
extern "C"
{
	MARKTECH_API int LaunchEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
}
#endif // MT_PLATFORM_WINDOWS