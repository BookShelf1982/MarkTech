#pragma once
#include <Windows.h>
#include <crtdbg.h>

class IModuleInterface
{
public:
	~IModuleInterface() {}
	virtual void PreInit(HINSTANCE hInstance) = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
protected:
	HINSTANCE hInstance;
};

extern "C"
{
	__declspec(dllexport) HRESULT GetInterface(IModuleInterface** pInterface);
	typedef HRESULT(*pfnGetInterface)(IModuleInterface** pInterface);

	__declspec(dllexport) HRESULT FreeInterface(IModuleInterface** pInterface);
	typedef HRESULT(*pfnFreeInterface)(IModuleInterface** pInterface);
}