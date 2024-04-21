#include "SandboxApp.h"
#include <crtdbg.h>

CSandboxApp::CSandboxApp()
	:m_pEngine(nullptr)
{
	std::vector<std::string> modules = GetModulesFromINI();
	m_pEngine = CreateMarkTechEngine(modules);
}

CSandboxApp::~CSandboxApp()
{
}

void CSandboxApp::OnInit()
{
}

void CSandboxApp::OnUpdate()
{
	m_pEngine->Update();

	if (m_pEngine->WantToQuit())
	{
		Quit();
	}
}

void CSandboxApp::OnQuit()
{
	m_pEngine->Release();
}
