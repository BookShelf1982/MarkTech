#pragma once
#include "Window.h"
#include <Windows.h>

class CWinWindow : public IWindow
{
public:
	CWinWindow();
	~CWinWindow();

	virtual void MakeWindow( String title, int x, int y, int width, int height, EWindowType type ) override;
	virtual void KillWindow() override;
	HWND GetHWND() { return m_hWnd; }
	void SetHInstance(HINSTANCE hInstacne);
private:
	HWND m_hWnd;
	WNDCLASS m_WndClass;
	HINSTANCE m_hInstance;
};