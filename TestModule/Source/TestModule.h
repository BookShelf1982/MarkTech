#pragma once
#include "ModulesShared.h"
extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

class CTestModules : public IModuleInterface
{
public:
	CTestModules();
	~CTestModules();

	virtual void PreInit(HINSTANCE hInstance) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Shutdown() override;

	static CTestModules* g_pTestModule;
private:
	lua_State* L = NULL;
};

extern "C"
{
	int lua_OutputDebugString(lua_State* L);
}