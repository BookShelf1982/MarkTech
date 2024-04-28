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
	MEngineData GetEngineDataFromINI(const CString& filepath);

private:
	static IMarkTechApplication* m_pInstance;

	CTList<CString> ParseINIArray(const std::string& string);

	bool m_bIsRunning;
};