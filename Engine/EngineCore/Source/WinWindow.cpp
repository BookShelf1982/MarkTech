#include "WinWindow.h"

CWinWindow::CWinWindow()
    :m_hInstance(NULL), m_WndClass(), m_hWnd(NULL)
{
}

CWinWindow::~CWinWindow()
{
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CWinWindow::MakeWindow(String title, int x, int y, int width, int height, EWindowType type, CInput* inputRef)
{
    m_pInputRef = inputRef;
	m_WndClass.lpszClassName = L"MarkTechWindow";
	m_WndClass.hInstance = m_hInstance;
    m_WndClass.lpfnWndProc = WindowProc;

    RegisterClass(&m_WndClass);

    m_nWidth = width;
    m_nHeight = height;
    m_nX = x;
    m_nY = y;
    m_pszTitle = title;
    m_hWnd = CreateWindowEx(0, m_WndClass.lpszClassName, L"MarkTech 2024", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, x, y, width, height, NULL, NULL, NULL, NULL);

    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);
}

void CWinWindow::KillWindow()
{
    DestroyWindow(m_hWnd);
    UnregisterClass(m_WndClass.lpszClassName, m_WndClass.hInstance);
}

void CWinWindow::MessageLoop(const MSG* pMsg)
{
    TranslateMessage(pMsg);
    DispatchMessageW(pMsg);
    switch (pMsg->message)
    {
    case WM_MOVE:
        m_nX = LOWORD(pMsg->lParam);
        m_nY = HIWORD(pMsg->lParam);
        break;
    case WM_SIZE:
        m_nWidth = LOWORD(pMsg->lParam);
        m_nHeight = HIWORD(pMsg->lParam);
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        bool bDown = (pMsg->lParam & 1 << 31) == 0;
        m_pInputRef->PollKeyboardInput((UINT)pMsg->wParam, bDown);
        break;
    }
}

void CWinWindow::SetHInstance(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
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
