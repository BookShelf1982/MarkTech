#include "SandboxApp.h"
#include <crtdbg.h>

CSandboxApp::CSandboxApp()
	:m_pEngine(nullptr)
{
	CString string = "Hello World!";
	/*CString hello = string.SubString(0, 5);
	CString hi = "Hi";
	hi = string;*/
	MEngineData engData = GetEngineDataFromINI("GameInfo.ini");
	m_pEngine = CreateMarkTechEngine(engData);
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
