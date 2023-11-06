#pragma once
#include "DllExport.h"
#include "Core.h"
#include "mini\ini.h"

namespace MarkTech
{
	namespace Configs
	{
		template<typename T>
		struct MARKTECH_API MConfigValue
		{
		public:
			MConfigValue() : value(NULL) {}
			MConfigValue(T InitalValue) : value(InitalValue) {}
			~MConfigValue() {}

			T value;
		};

		struct MARKTECH_API MGameInfo
		{
		public:
			static MGameInfo* GetGameInfo() { return m_gGInfo; }

			bool Init();

			void Destroy();

			MConfigValue<int>	gameversion;
			MConfigValue<char*> GameName;
			MConfigValue<char*> ShaderPath;
			MConfigValue<char*> RawShaderPath;

		private:
			MGameInfo() :gameversion(0), GameName(""), ShaderPath(""), RawShaderPath("") {}

			static MGameInfo* m_gGInfo;
		};

		struct MARKTECH_API MUserSettings
		{
		public:
			static MUserSettings* GetUserSettings() { return m_gUserSettings; }

			bool Init();

			void Destroy();

			MConfigValue<int> nVSWidth;
			MConfigValue<int> nVSLength;
			MConfigValue<int> bVSWindowed;
			MConfigValue<int> bVSVSync;
		private:
			MUserSettings() :nVSWidth(0), nVSLength(0), bVSWindowed(0), bVSVSync(0) {}

			static MUserSettings* m_gUserSettings;
		};
	}
}