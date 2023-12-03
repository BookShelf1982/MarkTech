#pragma once
#include "DllExport.h"
#include "Core.h"
#include "Level.h"
#include "D3D11Renderer.h"
#include "WinWindow.h"
#include "Configs.h"

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

		bool bIsEditor;

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
	};
}

#ifdef MT_PLATFORM_WINDOWS
extern "C"
{
	MARKTECH_API int LaunchEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

	MARKTECH_API int LaunchEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
}
#endif // MT_PLATFORM_WINDOWS