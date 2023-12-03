#pragma once
#include "DllExport.h"
#include "Core.h"
#include "Input.h"
#include <Windows.h>
#include <windowsx.h>
namespace MarkTech
{
	class MARKTECH_API CWinWindow
	{
	friend CInput;
	public:

		CWinWindow();
		~CWinWindow();

		void CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow);
		void MessageLoop(MSG msg);

		void CreateErrorBox(LPCWSTR ErrorString);

		HWND GetHWND() const { return m_Hwnd; }

		int nWidth, nHeight;

	private:
		HWND m_Hwnd;
		WNDCLASS m_WC;
	};
}