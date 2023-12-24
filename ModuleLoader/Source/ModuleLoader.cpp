#include "ModuleLoader.h"

HMODULE* LoadMModules()
{
	mINI::INIFile file("Bin/Modules/Modules.ini");
	mINI::INIStructure ini;

	bool b = file.read(ini);

	char** names = new char*[ini.size()];
	HMODULE* modules = new HMODULE[ini.size()];

	if (b)
	{
		for (size_t i = 0; i < ini.size(); i++)
		{
			char tempstr[256] = "";
			sprintf_s(tempstr, "LoadedModuleList[%zu]", i);
			strcpy_s(tempstr, 256, ini["Modules"][tempstr].c_str());
			modules[i] = LoadMModule(tempstr);
		}
	}
	else
	{
		delete[] names;
		return new HMODULE[1];
	}
	for (size_t i = 0; i < ini.size(); i++)
	{
		delete &names[i];
	}
	//delete[] names;
	return modules;
}

HMODULE LoadMModule(char* moduleName)
{
	char str[256] = "";
	sprintf_s(str, "Bin/Modules/%s.dll", (char*)moduleName);
	return LoadLibraryA(str);
}
