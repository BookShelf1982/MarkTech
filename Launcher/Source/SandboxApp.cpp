#include "SandboxApp.h"
#include <crtdbg.h>

CSandboxApp::CSandboxApp()
	:m_pEngine(nullptr), masterText(new test_t())
{
	std::vector<std::string> modules = GetModulesFromINI();
	m_pEngine = CreateMarkTechEngine(modules);
}

CSandboxApp::~CSandboxApp()
{
}

void CSandboxApp::OnInit()
{
	CTSharedPointer<test_t> text = masterText;
	CTSharedPointer<test_t> text2 = text;
	CTSharedPointer<test_t> text3 = text2;
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
