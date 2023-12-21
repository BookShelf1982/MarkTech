#include "Globals.h"
namespace MarkTech
{
	CGlobals* CGlobals::g_pGlobals = new CGlobals();

	void CGlobals::SetLevel(CLevel* level)
	{
		g_pGlobals->g_pLevel = level;
	}

	void CGlobals::SetInput(CInput* input)
	{
		g_pGlobals->g_pInput = input;
	}

	void CGlobals::SetGameInfo(MGameInfo* info)
	{
		g_pGlobals->g_pGameInfo = info;
	}

	void CGlobals::SetUserSettings(MUserSettings* settings)
	{
		g_pGlobals->g_pUserSettings = settings;
	}

	void CGlobals::Destroy()
	{
		delete g_pGlobals;
	}

	CGlobals::~CGlobals()
	{
	}
}