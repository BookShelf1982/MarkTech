#pragma once
#include <Windows.h>
typedef void (*pfnStopGameLoop)();

typedef unsigned int MTRESULT;

#define MT_OK ((MTRESULT)0);
#define MT_FAIL ((MTRESULT)1);


class IModuleInterface
{
public:
	~IModuleInterface() {}
	virtual void PreInit(HINSTANCE hInstance, pfnStopGameLoop pfn) = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;

protected:
	HINSTANCE m_hInstance;
	pfnStopGameLoop m_pfnStopGameLoop;
};

extern "C"
{
	__declspec(dllexport) MTRESULT GetInterface(IModuleInterface** pInterface);
	typedef MTRESULT(*pfnGetInterface)(IModuleInterface** pInterface);
}