#pragma once

#ifdef MT_PLATFORM_WINDOWS
	#ifdef MT_BUILD_DLL
		#define	MARKTECH_API __declspec(dllexport)
	#else
		#define	MARKTECH_API __declspec(dllimport)
	#endif
#else 
#error MarkTech only support Winodws!
#endif 

#define BIT(x) (1 << x)