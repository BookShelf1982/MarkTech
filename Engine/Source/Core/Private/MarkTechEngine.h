#pragma once
#include "Engine.h"
#include "Module.h"
#include "WindowInterface.h"
#include "ResourceManagerInterface.h"
#include "3DRenderer.h"

#ifdef MT_PLATFORM_WINDOWS
#include "WinModule.h"
#endif

#include <vector>

class CEngine : public IMarkTechEngine
{
public:
	virtual void Release() override;

	virtual void Update() override;

	virtual bool WantToQuit() override;

	void Quit();

	static CEngine* CreateEngine(const MEngineData& data);

	static window_funcs_t GetWindowFuncs() { return CEngine::m_pIstance->m_pfnWindowFuncs; }

private:
	CEngine(MEngineData engData);
	~CEngine();

	static void DestroyEngine();

	void Init();

	static CEngine* m_pIstance;

	bool m_bClosing;

	MEngineData m_Data;

	std::vector<IModule*> m_LoadedModules;

	window_funcs_t m_pfnWindowFuncs;
	resource_manager_funcs_t m_pfnResourceManagerFuncs;

	IModule* m_pWindowModule;
	IModule* m_pResourceManagerModule;

	// --  Window Stuff -- //
	GLFWwindow* m_pWindow;

	// -- Graphics -- //
	C3DRenderer* m_pRenderer;
	CTSharedPointer<IBuffer> m_Buffer;

	// -- Resource Management -- //
	IResourceManager* m_pResourceManager;
};

void WindowCloseCallback(GLFWwindow* pWindow);