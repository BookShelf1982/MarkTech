#include "WinWindow.h"

namespace MarkTech
{
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    CWinWindow::CWinWindow()
    {
        m_Hwnd = NULL;
        m_WC = { };
    }

    void CWinWindow::CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow)
    {
        m_WC.lpfnWndProc = WindowProc;
        m_WC.hInstance = hInstance;
        m_WC.lpszClassName = ClassName;

        RegisterClass(&m_WC);

        m_Hwnd = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, iPosX, iPosY, iWidth, iLength, NULL, NULL, hInstance, NULL);

        ShowWindow(m_Hwnd, nCmdShow);
        UpdateWindow(m_Hwnd);

        QueryPerformanceCounter(&nLastTick);
        QueryPerformanceFrequency(&nTickFrequency);
    }

    void CWinWindow::CreateErrorBox(LPCWSTR ErrorString)
    {
        MessageBox(m_Hwnd, ErrorString, L"MARKTECH FATAL ERROR!", MB_ICONWARNING);
    }

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32_t keycode = (uint32_t)wParam;

            bool bWasDown = (lParam & BIT(30)) != 0;
            bool bIsDown = (lParam & BIT(31)) == 0;

            CInput::GetInput()->PollInput(keycode, bIsDown, bWasDown);
        }break;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    void CWinWindow::MessageLoop(MSG msg)
    {
        QueryPerformanceCounter(&nCurrentTick);
        nElapsedTicks = nCurrentTick.QuadPart - nLastTick.QuadPart;

        nLastTick = nCurrentTick;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}