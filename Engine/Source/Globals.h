#pragma once
#include "Core.h"
#include "Level.h"
#include "Input.h"
#include "Configs.h"

namespace MarkTech
{
	class CGlobals
	{
	public:
		static CLevel* GetLevel() { return g_pGlobals->g_pLevel; }
		static CInput* GetInput() { return g_pGlobals->g_pInput; }
		static MGameInfo* GetGameInfo() { return g_pGlobals->g_pGameInfo; }
		static MUserSettings* GetUserSettings() { return g_pGlobals->g_pUserSettings; }

		static CGlobals* GetGlobals() { return g_pGlobals; }

		void SetLevel(CLevel* level);
		void SetInput(CInput* input);
		void SetGameInfo(MGameInfo* info);
		void SetUserSettings(MUserSettings* settings);

		void Destroy();
	private:
		CGlobals();

		~CGlobals();

		static CGlobals* g_pGlobals;

		CLevel* g_pLevel;
		CInput* g_pInput;
		MGameInfo* g_pGameInfo;
		MUserSettings* g_pUserSettings;
	};

	static CLevel* GetLevel() { return CGlobals::GetGlobals()->GetLevel(); }
	static CInput* GetInput() { return CGlobals::GetGlobals()->GetInput(); }
	static MGameInfo* GetGameInfo() { return CGlobals::GetGlobals()->GetGameInfo(); }
	static MUserSettings* GetUserSettings() { return CGlobals::GetGlobals()->GetUserSettings(); }
}
