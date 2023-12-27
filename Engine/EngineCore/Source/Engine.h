#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "Level.h"
#include "Input.h"

typedef void (*pfnCloseGame)();

class CEngine
{
public:
	CEngine();
	~CEngine();

	void PreInitEngine(HINSTANCE hInstance);
	bool InitEngine(pfnCloseGame pfn);
	void DestroyEngine();
	void UpdateEngine();

	pfnCloseGame CloseGame;
private:
	HINSTANCE m_hInstance;
	CWinWindow* m_pWindow;
	CLevel* m_pLevel;
	CInput* m_pInput;
};