#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "D3Drenderer.h"
#include "Map.h"
#include "PointEntity.h"

class CEngine
{
public:
	CEngine();
	~CEngine();

	void InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

	void StartEngineLoop();

private:

	bool bClosing;
	LARGE_INTEGER nTickFrequency, nCurrentTick, nLastTick;
	int64_t nElapsedTicks;
	double flDeltaTime;
	CLog* Logger;
	CWinWindow* Window;
	CD3DRenderer* Renderer;
	CMap* Map;
};
