#ifdef MT_BUILD_DLL
#define MT_DLLEXPORT extern "C" __declspec(dllexport)
#else
#define MT_DLLEXPORT
#endif
