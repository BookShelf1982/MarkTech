#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "Level.h"
#include "Input.h"
#include "DX11Renderer.h"
#include "AssetRegistry.h"
#include <future>
#include <thread>

class CEngine
{
public:
	CEngine();
	~CEngine();

	void PreInitEngine(HINSTANCE hInstance);
	bool InitEngine();
	void DestroyEngine();
	void StartEngineLoop();
	void RenderLoop();
	void Close();

	bool IsClosing() const { return m_bClosing; }
private:
	std::atomic<bool> m_bClosing;
	HINSTANCE m_hInstance;
	CWinWindow* m_pWindow;
	CLevel* m_pLevel;
	CInput* m_pInput;
	IRenderer* m_pRenderer;
	CAssetRegistry* m_pAssetRegistry;

	std::thread m_RenderThread;
};