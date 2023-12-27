#pragma once
#include "StringType.h"

enum EWindowType
{
	EFullscreen,
	EBorderless,
	EWindowed
};

class IWindow
{
public:
	virtual void MakeWindow( String title, int x, int y, int width, int height, EWindowType type ) = 0;
	virtual void KillWindow() = 0;
protected:
	String m_Title;
	int m_nX;
	int m_nY;
	int m_nWidth;
	int m_nHeight;
	EWindowType m_Type;
};