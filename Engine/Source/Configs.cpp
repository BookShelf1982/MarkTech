#include "Configs.h"

namespace MarkTech
{
	namespace Configs
	{
		MGameInfo* MGameInfo::m_gGInfo;

		MUserSettings* MUserSettings::m_gUserSettings;

		bool MGameInfo::Init()
		{
			mINI::INIFile file("GameInfo.ini");
			mINI::INIStructure ini;

			file.read(ini);

			std::string string = ini["MarkTech.GameInfo"]["Game.GameName"];


			return true;
		}

		void MGameInfo::Destroy()
		{
			delete m_gGInfo;
		}

		bool MUserSettings::Init()
		{
			mINI::INIFile file("GameInfo.ini");
			mINI::INIStructure ini;

			file.read(ini);

			std::string string = ini["MarkTech.GameInfo"]["Game.GameName"];

			return true;
		}

		void MUserSettings::Destroy()
		{
			delete m_gUserSettings;
		}

	}
}