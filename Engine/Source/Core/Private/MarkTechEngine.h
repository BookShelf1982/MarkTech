#pragma once
#include "Engine.h"
#include "Module.h"
#include "WindowInterface.h"
#include "GraphicsInterface.h"

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

	static CEngine* CreateEngine(const std::vector<std::string>& modules);

	static window_funcs_t GetWindowFuncs() { return CEngine::m_pIstance->m_pfnWindowFuncs; }

private:
	CEngine(const std::vector<std::string>& modules);
	~CEngine();

	static void DestroyEngine();

	void Init();

	static CEngine* m_pIstance;

	bool m_bClosing;

	std::vector<IModule*> m_LoadedModules;

	window_funcs_t m_pfnWindowFuncs;
	graphics_funcs_t m_pfnGraphicsFuncs;

	IModule* m_pWindowModule;
	IModule* m_pGraphicsModule;

	// --  Window Stuff -- //
	GLFWwindow* m_pWindow;

	// -- graphics -- //
	IGraphicsContext* m_pGraphicsContext;
};

void WindowCloseCallback(GLFWwindow* pWindow);