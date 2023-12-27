#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "Level.h"

typedef void (*pfnCloseGame)();

class CEngine
{
public:
	CEngine();
	~CEngine();

	bool InitEngine(pfnCloseGame pfn);
	void DestroyEngine();
	void UpdateEngine();

	pfnCloseGame CloseGame;
private:
	IWindow* m_pWindow;
	CLevel* m_pLevel;
};