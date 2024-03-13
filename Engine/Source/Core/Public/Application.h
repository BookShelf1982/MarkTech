#pragma once
#include "Core.h"
#include "Engine.h"
#include <string>
#include <vector>

class IMarkTechApplication
{
public:
	IMarkTechApplication();
	virtual ~IMarkTechApplication();

	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnQuit() {}

	void Run();
	void Update();
	void Quit();

	static IMarkTechApplication* Get();

private:
	static IMarkTechApplication* m_pInstance;

	std::vector<std::string> ParseINIArray(const std::string& string);

	bool m_bIsRunning;
	IMarkTechEngine* m_pEngine;
};