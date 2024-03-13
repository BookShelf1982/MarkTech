#pragma once
#include <stdint.h>

extern "C"
{
	void CreateWindow(const char* pTitle, uint32_t nWidth, uint32_t nHeight);

	void PollWindowEvents();

	void CloseWindow();
}