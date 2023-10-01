#pragma once
#include "Core.h"
#include "WinWindow.h"


class CEngine
{
public:
	CEngine();
	~CEngine();

	void InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);

	void StartEngineLoop();

private:

	bool bClosing;
	CLog* Logger;
	CWinWindow* Window;
};
