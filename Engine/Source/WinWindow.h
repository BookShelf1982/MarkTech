#pragma once
#include <Windows.h>

class CWinWindow
{
public:

	void CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow);

	void MessageLoop(MSG msg);
};

