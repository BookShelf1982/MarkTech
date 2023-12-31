#pragma once
#include "Core.h"
#include "Input.h"

enum EWindowType
{
	EFullscreen,
	EBorderless,
	EWindowed
};

class IWindow
{
public:
	virtual void MakeWindow( String title, int x, int y, int width, int height, EWindowType type, CInput* m_pInputRef ) = 0;
	virtual void KillWindow() = 0;

	String GetTitle() { return m_pszTitle; }
	int GetXPos() { return m_nX; }
	int GetYPos() { return m_nY; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }
protected:
	String m_pszTitle;
	int m_nX;
	int m_nY;
	int m_nWidth;
	int m_nHeight;
	EWindowType m_Type;
	CInput* m_pInputRef;
};