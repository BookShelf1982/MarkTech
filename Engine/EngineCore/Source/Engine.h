#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "Level.h"
#include "Input.h"
#include "DX11Renderer.h"

class CEngine
{
public:
	CEngine();
	~CEngine();

	void PreInitEngine(HINSTANCE hInstance);
	bool InitEngine();
	void DestroyEngine();
	void StartEngineLoop();
	void Close();
private:
	HINSTANCE m_hInstance;
	CWinWindow* m_pWindow;
	CLevel* m_pLevel;
	CInput* m_pInput;
	IRenderer* m_pRenderer;
	bool m_bClosing;
};