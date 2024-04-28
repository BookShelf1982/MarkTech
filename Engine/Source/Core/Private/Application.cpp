#include "Application.h"
#define MINI_CASE_SENSITIVE
#include <ini\ini.h>

IMarkTechApplication* IMarkTechApplication::m_pInstance = nullptr;

IMarkTechApplication::IMarkTechApplication()
	:m_bIsRunning(true)
{
	MASSERT(!m_pInstance);
	m_pInstance = this;
}

void IMarkTechApplication::Run()
{
	OnInit();
	while (m_bIsRunning)
	{
		OnUpdate();
	}
	OnQuit();
}

void IMarkTechApplication::Quit()
{
	m_bIsRunning = false;
}

IMarkTechApplication* IMarkTechApplication::Get()
{
	MASSERT(m_pInstance);
	return m_pInstance;
}

MEngineData IMarkTechApplication::GetEngineDataFromINI(const CString& filepath)
{
	mINI::INIFile file(filepath.c_str());
	mINI::INIStructure iniStruct;
	MASSERT(file.read(iniStruct)); // Read ini file
	MASSERT(iniStruct.has("MarkTech.GameInfo"));
	MASSERT(iniStruct.has("MarkTech.Modules"));
	MASSERT(iniStruct["MarkTech.Modules"].has("RequiredModules"));

	MEngineData engData;
	if (iniStruct["MarkTech.GameInfo"].has("GameName"))
		engData.gameName = iniStruct["MarkTech.GameInfo"]["GameName"].c_str();
	if (iniStruct["MarkTech.GameInfo"].has("DefaultMap"))
		engData.defaultMap = iniStruct["MarkTech.GameInfo"]["DefaultMap"].c_str();
	
	std::string iniArray = iniStruct["MarkTech.Modules"]["RequiredModules"];
	CTList<CString> moduleNames = ParseINIArray(iniArray);
	engData.modules = moduleNames;

	return engData;
}

CTList<CString> IMarkTechApplication::ParseINIArray(const std::string& string)
{
	// -- remove curly braces -- //
	CString String2 = string.c_str();
	CString newString = String2.SubString(1, String2.Size() - 2);
	CTList<CString> strings;

	for (uint32_t i = 0; i < newString.Size();)
	{
		uint32_t nStartPos = i;
		uint32_t nEndPos = newString.FindFirstOf(',', i);
		
		if (nEndPos == 0)
			nEndPos = newString.Size();

		uint32_t nLength = nEndPos - nStartPos;
		CString element = newString.SubString(nStartPos, nLength);
		i = nStartPos + nLength + 1;

		strings.Push(element);
	}

	return strings;
}