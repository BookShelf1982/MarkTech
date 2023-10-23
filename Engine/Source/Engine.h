#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "Input.h"
#include "D3Drenderer.h"
#include "Map.h"
#include "imgui.h"
#include "GameInfo.h"
#include "UserSettings.h"

class CEngine
{
public:

	CEngine();
	~CEngine();

	bool InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

	bool InitImGui(HWND hwnd);

	bool InitEngineEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

	bool ReadConfigFiles();

	void StartEngineLoop();

private:

	bool bClosing;
	bool bShowDemoWindow;
	UINT nResizeWidth = 0, nResizeHeight = 0;
	LARGE_INTEGER nTickFrequency, nCurrentTick, nLastTick;
	int64_t nElapsedTicks;
	float flDeltaTime;
	CWinWindow* Window;
	CMap* Map;
};

