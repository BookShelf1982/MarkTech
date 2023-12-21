#include "Configs.h"

namespace MarkTech
{
	MGameInfo::MGameInfo()
		:nGameVersion(0), szGameName(MT_NAME), szShaderPath(MT_SHADERPATH), szContentPath(MT_CONTENTPATH), szConfigPath(MT_CONFIGPATH)
	{
	}

	MGameInfo::~MGameInfo()
	{
	}

	bool MGameInfo::Init()
	{
		mINI::INIFile file("GameInfo.ini");
		mINI::INIStructure ini;

		bool bReadFile = file.read(ini);

		if (!bReadFile)
		{
			return false;
		}

		std::string tempGameName = ini["MarkTech.GameInfo"]["Game.GameName"];
		std::string tempShaderPath = ini["MarkTech.GameInfo"]["Game.ShaderPath"];
		std::string tempContentPath = ini["MarkTech.GameInfo"]["Game.ContentPath"];
		std::string tempConfigPath = ini["MarkTech.GameInfo"]["Game.ConfigPath"];

		strcpy(this->szGameName, tempGameName.c_str());
		strcpy(this->szShaderPath, tempShaderPath.c_str());
		strcpy(this->szContentPath, tempContentPath.c_str());
		strcpy(this->szConfigPath, tempConfigPath.c_str());

		
		return true;
	}

	MUserSettings::MUserSettings()
		:nVSWidth(0), nVSHeight(0), bVSWindowed(0), bVSVSync(0)
	{
	}

	MUserSettings::~MUserSettings()
	{
	}

	bool MUserSettings::Init(const MGameInfo& gameinfo)
	{
		char* szPath = (char*)gameinfo.GetConfigPath();
		char szFile[256] = "UserSettings.ini";
		strcat(szPath, szFile);
		mINI::INIFile file(szPath);
		mINI::INIStructure ini;

		if (!file.read(ini))
		{
			ini["MarkTech.VideoSettings"]["VideoSettings.Width"] = "800";
			ini["MarkTech.VideoSettings"]["VideoSettings.Height"] = "600";
			ini["MarkTech.VideoSettings"]["VideoSettings.Windowed"] = "1";
			ini["MarkTech.VideoSettings"]["VideoSettings.VSync"] = "1";

			if (!file.generate(ini))
				return false;

			return true;
		}

		std::string widthStr = ini["MarkTech.VideoSettings"]["VideoSettings.Width"];
		std::string heightStr = ini["MarkTech.VideoSettings"]["VideoSettings.Height"];
		std::string windowStr = ini["MarkTech.VideoSettings"]["VideoSettings.Windowed"];
		std::string vsyncStr = ini["MarkTech.VideoSettings"]["VideoSettings.VSync"];

		char *charPtr;
		this->nVSWidth = (int)strtol(widthStr.c_str(), &charPtr, 10);
		this->nVSHeight = (int)strtol(heightStr.c_str(), &charPtr, 10);
		this->bVSWindowed = (int)strtol(windowStr.c_str(), &charPtr, 10);
		this->bVSVSync = (int)strtol(vsyncStr.c_str(), &charPtr, 10);

		return true;
	}
}