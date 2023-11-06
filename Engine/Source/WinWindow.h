#pragma once
#include "DllExport.h"
#include "Core.h"
#include <Windows.h>

class MARKTECH_API CWinWindow
{
public:

	CWinWindow();

	void CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow);
	void MessageLoop(MSG msg);

	void CreateErrorBox(LPCWSTR ErrorString);

	HWND GetHWND() { return hwnd; }

private:
	HWND hwnd;
	WNDCLASS wc;
};

