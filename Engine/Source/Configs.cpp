#include "Configs.h"

namespace MarkTech
{
	MGameInfo* MGameInfo::m_gGInfo = new MGameInfo();

	MUserSettings* MUserSettings::m_gUserSettings = new MUserSettings();

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
		std::string tempRawShaderPath = ini["MarkTech.GameInfo"]["Game.RawShaderPath"];
		std::string tempContentPath = ini["MarkTech.GameInfo"]["Game.ContentPath"];
		std::string tempConfigPath = ini["MarkTech.GameInfo"]["Game.ConfigPath"];
		std::string tempImage = ini["MarkTech.GameInfo"]["Game.Image"];
		std::string tempModel = ini["MarkTech.GameInfo"]["Game.Model"];

		strcpy(this->szGameName, tempGameName.c_str());
		strcpy(this->szShaderPath, tempShaderPath.c_str());
		strcpy(this->szRawShaderPath, tempRawShaderPath.c_str());
		strcpy(this->szContentPath, tempContentPath.c_str());
		strcpy(this->szConfigPath, tempConfigPath.c_str());
		strcpy(this->szImage, tempImage.c_str());
		strcpy(this->szModel, tempModel.c_str());

		
		return true;
	}

	void MGameInfo::Destroy()
	{
		delete m_gGInfo;
	}

	bool MUserSettings::Init()
	{
		char* szPath = MGameInfo::GetGameInfo()->szConfigPath;
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

	void MUserSettings::Destroy()
	{
		delete m_gUserSettings;
	}
}