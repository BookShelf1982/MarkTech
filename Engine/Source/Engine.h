#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "D3Drenderer.h"
#include "Map.h"
#include "GameInfo.h"

class CEngine
{
public:
	CEngine();
	~CEngine();

	bool InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

	void StartEngineLoop();



private:

	bool bClosing;
	LARGE_INTEGER nTickFrequency, nCurrentTick, nLastTick;
	int64_t nElapsedTicks;
	float flDeltaTime;
	CWinWindow* Window;
	CD3DRenderer* Renderer;
	CMap* Map;
};

