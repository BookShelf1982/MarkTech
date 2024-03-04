#pragma once
#include "Core.h"
#include "WinWindow.h"
#include "Level.h"
#include "Input.h"
#include "3DRenderer.h"
#include "AssetRegistry.h"
#include <atomic>
#include <thread>

struct MDATA
{
	int num = 64;
};

class CEngine
{
public:
	CEngine();
	~CEngine();

	void PreInitEngine(HINSTANCE hInstance);
	bool InitEngine();
	void DestroyEngine();
	void StartEngineLoop();
	void GameLoop();
	void Close();

	bool IsClosing() const { return m_bClosing; }
private:
	std::atomic<bool> m_bClosing;
	HINSTANCE m_hInstance;
	CWinWindow* m_pWindow;
	CLevel* m_pLevel;
	CInput* m_pInput;
	C3DRenderer* m_pRenderer;
	CAssetRegistry* m_pAssetRegistry;

	std::thread m_GameThread;
};