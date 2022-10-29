#pragma once

#ifdef MT_PLATFORM_WINDOWS
	#ifdef MT_BUILD_DLL
		#define MARKTECH_API __declspec(dllexport)
	#else
		#define MARKTECH_API __declspec(dllimport)
	#endif
#else

#error MARKTECH ONLY SUPPORTS WINDOWS

#endif // MT_PLATFORM_WINDOS

#define BIT(x) (1 << x)

#ifdef MT_ENABLE_ASSERTS
#define MT_ASSERT(x, ...) {if(!(x)) {MT_ERROR("Assertion Failed: {0}", __VA_ARGS__	); __debugbreak();}}
#define MT_CORE_ASSERT(x, ...) {if(!(x)) {MT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__	); __debugbreak();}}
#else
#define MT_ASSERT(x, ...) 
#define MT_CORE_ASSERT(x, ...) 
#endif
