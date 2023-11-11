#pragma once
#include "DllExport.h"
#include "Core.h"
#define MINI_CASE_SENSITIVE
#include "mini\ini.h"

namespace MarkTech
{
	namespace Configs
	{
#define MT_NAME "MarkTech 2023"

#define MT_SHADERPATH "Bin/Shaders/"

#define MT_RAWSHADERPATH "Bin/Shaders/Source/"

#define MT_CONTENTPATH "Content/"

#define MT_CONFIGPATH "Configs/"

		struct MARKTECH_API MGameInfo
		{
		public:
			static MGameInfo* GetGameInfo() { return m_gGInfo; }

			bool Init();

			void Destroy();

			int		gameversion;
			char	GameName[256];
			char	ShaderPath[256];
			char	RawShaderPath[256];
			char	ContentPath[256];
			char	ConfigPath[256];

		private:
			MGameInfo() :gameversion(0), GameName(MT_NAME), ShaderPath(MT_SHADERPATH), RawShaderPath(MT_RAWSHADERPATH), ContentPath(MT_CONTENTPATH), ConfigPath(MT_CONFIGPATH) {}
			~MGameInfo() {}

			static MGameInfo* m_gGInfo;
		};

		struct MARKTECH_API MUserSettings
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
}