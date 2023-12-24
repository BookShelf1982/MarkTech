#include "TestModule.h"

CTestModules* CTestModules::g_pTestModule = new CTestModules();;

CTestModules::CTestModules()
{
}

CTestModules::~CTestModules()
{
	if (L)
		lua_close(L);
}

void CTestModules::PreInit(HINSTANCE hInstance)
{
	OutputDebugStringA("TestModule:PreInit\n");
}

void CTestModules::Init()
{
	OutputDebugStringA("TestModule:Init\n");
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "_WinOutputString", lua_OutputDebugString);

	int lr = luaL_dofile(L, "test.lua");
	if (lr == LUA_OK)
	{
		lua_getglobal(L, "Start");
		if (lua_isfunction(L, -1))
		{
			lua_pcall(L, 0, 0, 0);
		}
	}
	else
	{
		const char* err = lua_tostring(L, -1);
		OutputDebugStringA(err);
	}
}

void CTestModules::Update()
{
	OutputDebugStringA("TestModule:UpdateInit\n");
}

void CTestModules::Shutdown()
{
	OutputDebugStringA("TestModule:ShutdownInit\n");
	delete g_pTestModule;
}

int lua_OutputDebugString(lua_State* L)
{
	const char* str = lua_tostring(L, 1);
	char out[512];
	sprintf_s(out, "[LUA]: %s\n", str);
	OutputDebugStringA(out);
	return 0;
}

extern "C"
{
	HRESULT GetInterface(IModuleInterface** pInterface)
	{
		*pInterface = CTestModules::g_pTestModule;
		return S_OK;
	}

	HRESULT FreeInterface(IModuleInterface** pInterface)
	{
		OutputDebugStringA("Idk what to put here. CALL Shutdown funtion");
		return E_FAIL;
	}
}