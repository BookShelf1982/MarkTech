#include "WinWindow.h"

CWinWindow::CWinWindow()
    :m_hInstance(NULL), m_WndClass(), m_hWnd(NULL)
{
}

CWinWindow::~CWinWindow()
{
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CWinWindow::MakeWindow(String title, int x, int y, int width, int height, EWindowType type)
{
	m_WndClass.lpszClassName = L"MarkTechWindow";
	m_WndClass.hInstance = m_hInstance;
    m_WndClass.lpfnWndProc = WindowProc;

    RegisterClass(&m_WndClass);

    m_nWidth = width;
    m_nHeight = height;
    m_nX = x;
    m_nY = y;
    m_hWnd = CreateWindowEx(0, L"MarkTechWindow", L"MarkTech 2024", WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, m_hInstance, NULL);

    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);
}

void CWinWindow::KillWindow()
{
    DestroyWindow(m_hWnd);
    UnregisterClassW(m_WndClass.lpszClassName, m_WndClass.hInstance);
}

void CWinWindow::SetHInstance(HINSTANCE hInstacne)
{
    m_hInstance = hInstacne;
}

LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
