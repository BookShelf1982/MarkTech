#pragma once
#include <Application.h>

class CSandboxApp : public IMarkTechApplication
{
public:
	CSandboxApp();
	~CSandboxApp();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnQuit() override;

private:
	IMarkTechEngine* m_pEngine;
};