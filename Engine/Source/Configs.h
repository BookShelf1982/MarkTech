#pragma once
#include "DllExport.h"
#include "Core.h"
#define MINI_CASE_SENSITIVE
#include "mini\ini.h"

namespace MarkTech
{
#define MT_NAME "MarkTech 2023"

#define MT_SHADERPATH "Bin/Shaders/"

#define MT_RAWSHADERPATH "Bin/Shaders/Source/"

#define MT_CONTENTPATH "Content/"

#define MT_CONFIGPATH "Configs/"

	class MARKTECH_API MGameInfo
	{
	public:
		static MGameInfo* GetGameInfo() { return m_gGInfo; }

		bool Init();

		void Destroy();

		int		nGameVersion;
		char	szGameName[256];
		char	szShaderPath[256];
		char	szRawShaderPath[256];
		char	szContentPath[256];
		char	szConfigPath[256];
		char	szImage[256];
		char	szModel[256];

	private:
		MGameInfo() :nGameVersion(0), szGameName(MT_NAME), szShaderPath(MT_SHADERPATH), szRawShaderPath(MT_RAWSHADERPATH), szContentPath(MT_CONTENTPATH), szConfigPath(MT_CONFIGPATH), szImage("img.jpg"), szModel("") {}
		~MGameInfo() {}

		static MGameInfo* m_gGInfo;
	};

	class MARKTECH_API MUserSettings
	{
	public:
		static MUserSettings* GetUserSettings() { return m_gUserSettings; }

		bool Init();

		void Destroy();

		int nVSWidth;
		int nVSHeight;
		int bVSWindowed;
		int bVSVSync;
	private:
		MUserSettings() :nVSWidth(0), nVSHeight(0), bVSWindowed(0), bVSVSync(0) {}

		static MUserSettings* m_gUserSettings;
	};
}