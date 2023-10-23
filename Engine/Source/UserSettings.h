#pragma once
#include "Core.h"
#include "inipp.h"
#include <fstream>

struct MUserSettings
{
public:
	static MUserSettings& GetUserSettings() { return m_UserSettings; }

	bool Init();

	int nVSWidth;
	int nVSLength;
	int bVSWindowed;
	int bVSVSync;
private:
	MUserSettings() :nVSWidth(0), nVSLength(0), bVSWindowed(0), bVSVSync(0) {}

	static MUserSettings m_UserSettings;
};