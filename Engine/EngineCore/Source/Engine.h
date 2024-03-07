#pragma once
#include "Core.h"
#include "Level.h"
#include "Input.h"
#include "3DRenderer.h"
#include "AssetRegistry.h"
#include <atomic>
#include <thread>

class CEngine
{
public:
	CEngine();
	~CEngine();

	bool InitEngine();
	void DestroyEngine();
	void StartEngineLoop();
	void GameLoop();
	static void Close();

	bool IsClosing() const { return m_bClosing; }

	static void SetEnginePtr(CEngine* engine);
private:
	CMemoryPool m_EngineMemoryPool;

	static CEngine* m_pEngine;

	std::atomic<bool> m_bClosing;
	GLFWwindow* m_pWindow;
	CLevel* m_pLevel;
	CInput* m_pInput;
	C3DRenderer* m_pRenderer;
	CAssetRegistry* m_pAssetRegistry;

	std::thread m_GameThread;
};

void OnWindowClose(GLFWwindow* window);