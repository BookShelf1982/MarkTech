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

std::vector<std::string> IMarkTechApplication::GetModulesFromINI()
{
	mINI::INIFile file("Modules.ini");
	mINI::INIStructure moduleStruct;

	MASSERT(file.read(moduleStruct));

	bool bHasModulesKey = moduleStruct["Modules"].has("RequiredModules");
	MASSERT(bHasModulesKey);

	std::string iniArray = moduleStruct["Modules"]["RequiredModules"];

	std::vector<std::string> modules = ParseINIArray(iniArray);

	return modules;
}

std::vector<std::string> IMarkTechApplication::ParseINIArray(const std::string& string)
{
	// -- remove curly braces -- //
	std::string newString = string.substr(1, string.length() - 2);

	std::vector<std::string> strings;

	bool bFirstString = true;
	for (size_t i = 0; i < newString.length();)
	{
		std::string element;
		size_t nOffsetIndex = i;
		size_t nCommaIndex = newString.find_first_of(",", nOffsetIndex + 1);
		size_t nRange = nCommaIndex - nOffsetIndex;
		size_t nSearchPos = nOffsetIndex;
		
		if (!bFirstString)
		{
			nSearchPos++;
			nRange--;
		}

		element = newString.substr(nSearchPos, nRange);

		i = nCommaIndex;

		if (bFirstString)
			bFirstString = false;

		strings.emplace_back(element.c_str());
	}

	return strings;
}
