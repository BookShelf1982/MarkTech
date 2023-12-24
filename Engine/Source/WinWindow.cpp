#include "WinWindow.h"

namespace MarkTech
{
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    CWinWindow::CWinWindow()
    {
        m_Hwnd = NULL;
        m_WC = { };
    }

    CWinWindow::~CWinWindow()
    {
        DestroyWindow(m_Hwnd);
        UnregisterClassW(m_WC.lpszClassName, m_WC.hInstance);
    }

    void CWinWindow::CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow)
    {
        m_WC.lpfnWndProc = WindowProc;
        m_WC.hInstance = hInstance;
        m_WC.lpszClassName = ClassName;

        RegisterClass(&m_WC);

        nWidth = iWidth;
        nHeight = iLength;
        m_Hwnd = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, iPosX, iPosY, iWidth, iLength, NULL, NULL, hInstance, NULL);

        ShowWindow(m_Hwnd, nCmdShow);
        UpdateWindow(m_Hwnd);

        //ShowCursor(false);
    }

    void CWinWindow::CreateErrorBox(const LPCWSTR ErrorString)
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
        }break;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    void CWinWindow::MessageLoop(MSG msg)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        switch (msg.message)
        {
        case WM_SIZE:
        {
            nWidth = LOWORD(msg.lParam);
            nHeight = HIWORD(msg.lParam);
        }break;
        }
    }
}