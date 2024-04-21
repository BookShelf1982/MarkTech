#pragma once
#include "Core.h"
#include "Engine.h"
#include "Module.h"
#include <string>
#include <vector>

class IMarkTechApplication
{
public:
	IMarkTechApplication();
	virtual ~IMarkTechApplication() {}

	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnQuit() {}

	void Run();
	void Update();
	void Quit();

	static IMarkTechApplication* Get();

protected:
	std::vector<std::string> GetModulesFromINI();
	std::vector<IModule*> LoadModules(const std::vector<std::string>& modules);

private:
	static IMarkTechApplication* m_pInstance;

	std::vector<std::string> ParseINIArray(const std::string& string);

	bool m_bIsRunning;
};