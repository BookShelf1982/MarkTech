#pragma once
#ifdef MT_PLATFORM_WINDOWS
#ifdef MT_BUILD_DLL
#define MARKTECH_API __declspec(dllexport)
#else
#define MARKTECH_API __declspec(dllimport)
#endif //MT_BUILD_DLL

#else
#error MARKTECH ONLY SUPPORTS WINDOWS
#endif //MT_PLATFORM_WINDOWS