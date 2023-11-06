#pragma once
#include "DllExport.h"
#include "Core.h"
#include "Level.h"
#include "D3D11Renderer.h"
#include "WinWindow.h"
#include "Input.h"
#include "Configs.h"

namespace MarkTech
{
	class CEngine
	{
	public:

		void Quit();

		bool InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

		bool InitEngineEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

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
		UINT nResizeWidth = 0, nResizeHeight = 0;
		LARGE_INTEGER nTickFrequency, nCurrentTick, nLastTick;
		int64_t nElapsedTicks;
		float flDeltaTime;
		CWinWindow* Window;
	};
}

#ifdef MT_PLATFORM_WINDOWS
extern "C"
{
	MARKTECH_API int LaunchEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
}
#endif // MT_PLATFORM_WINDOWS