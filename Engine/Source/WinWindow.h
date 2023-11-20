#pragma once
#include "DllExport.h"
#include "Core.h"
#include "Input.h"
#include <Windows.h>
namespace MarkTech
{
	class MARKTECH_API CWinWindow
	{
	public:

		CWinWindow();

		void CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow);
		void MessageLoop(MSG msg);



		void CreateErrorBox(LPCWSTR ErrorString);

		HWND GetHWND() { return m_Hwnd; }

		LARGE_INTEGER nTickFrequency, nCurrentTick, nLastTick;
		int64_t nElapsedTicks;

	private:
		HWND m_Hwnd;
		WNDCLASS m_WC;
	};
}