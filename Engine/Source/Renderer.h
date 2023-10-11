#pragma once
#include "Core.h"
#include <Windows.h>

class CRenderer
{
public:
	virtual void InitRenderer(HWND hwnd) {};

	virtual void CreateShaders() {};

	virtual void RenderFrame(HWND hwnd) {};
};

