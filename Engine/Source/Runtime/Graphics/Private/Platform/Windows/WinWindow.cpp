#include "Platform\Windows\WinWindow.h"

namespace MarkTech
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_CLOSE:
			return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	WinWindow::WinWindow(WindowProperties props)
		:m_Properties(props), m_Window(NULL)
	{
		const wchar_t CLASS_NAME[] = L"MarkTechWindow";

		WNDCLASS windowClass = {};
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.lpszClassName = CLASS_NAME;
		windowClass.lpfnWndProc = WindowProc;

		RegisterClass(&windowClass);

		m_Window = CreateWindow(
			CLASS_NAME,
			props.pTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			props.width, props.height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL
		);

		ShowWindow(m_Window, SW_SHOWDEFAULT);
	}

	WinWindow::~WinWindow()
	{
		Destroy();
	}

	void WinWindow::SetEventHandler(PF_WINDOWEVENTHANDLER pfn)
	{
	}

	void WinWindow::PollWindowEvents()
	{
		MSG msg = {};
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	WindowProperties WinWindow::GetProperties() const
	{
		return m_Properties;
	}

	void WinWindow::Destroy()
	{
		DestroyWindow(m_Window);
		UnregisterClass(L"MarkTechWindow", GetModuleHandle(NULL));
	}


}