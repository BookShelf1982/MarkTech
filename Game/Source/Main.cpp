#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	typedef int (*pfnLaunchEngine)(HINSTANCE, PWSTR, int);	//Formatting for LaunchEngine Function

	HMODULE Library = LoadLibraryA("Bin/Engine.dll");

	if (Library == NULL)
	{
		MessageBox(NULL, L"FAILED TO FIND RUNTIME LIBRARY ( Engine.dll )", L"MARKTECH FATAL ERROR!", MB_ICONERROR);
		return -1;
	}
	else
	{
		pfnLaunchEngine pfnLaunchEng = (pfnLaunchEngine)GetProcAddress(Library, "LaunchEngine");

		if (pfnLaunchEng == NULL)
		{
			MessageBox(NULL, L"FAILED TO FIND RUNTIME LIBRARY FUNTION ( LaunchEngine() )", L"MARKTECH FATAL ERROR!", MB_ICONERROR);
			return -1;
		}
		else
		{
			pfnLaunchEng(hInstance, pCmdLine, nCmdShow);	//Call LaunchEngine Function
			FreeLibrary(Library);	//Free library when shut down
			return 0;
		}
	}
}