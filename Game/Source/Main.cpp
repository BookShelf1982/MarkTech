#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include <Windows.h>
#include <Engine.h>
#include <fstream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	typedef int (*DLLFunc)(HINSTANCE, PWSTR, int);	//Formatting for LaunchEngine Function

	DWORD error = 0;
	HWND hWnd = NULL;
	HMODULE Library = LoadLibraryA("Bin/Engine.dll");

	if (Library == NULL)
	{
		MessageBox(hWnd, L"FAILED TO FIND RUNTIME LIBRARY ( Engine.dll )", L"MARKTECH FATAL ERROR!", MB_ICONERROR);
		return -1;
	}
	else
	{
		DLLFunc func = (DLLFunc)GetProcAddress(Library, "LaunchEngine");
		if (func == NULL)
		{
			error = GetLastError();
			MessageBox(hWnd, L"FAILED TO FIND RUNTIME LIBRARY FUNTION ( LaunchEngine() )", L"MARKTECH FATAL ERROR!", MB_ICONERROR);
			return -1;
		}
		else
		{
			func(hInstance, pCmdLine, nCmdShow);	//Call LaunchEngine Function
			FreeLibrary(Library);	//Free library when shut down
			return 0;
		}
	}
}