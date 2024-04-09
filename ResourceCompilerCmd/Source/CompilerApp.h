#pragma once
#include <Application.h>
#include <Module.h>


class CCompilerApp : public IMarkTechApplication
{
public:
	CCompilerApp();
	~CCompilerApp();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnQuit() override;

private:
	IModule* m_pResourceCompilerModule;
};