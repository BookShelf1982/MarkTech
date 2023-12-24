#pragma once
#include "DllExport.h"
#include "Core.h"
#define MINI_CASE_SENSITIVE
#include "ini.h"

namespace MarkTech
{
#define MT_NAME "MarkTech 2023"

#define MT_SHADERPATH "Bin/Shaders/"

#define MT_CONTENTPATH "Content/"

#define MT_CONFIGPATH "Configs/"

	class MARKTECH_API MGameInfo
	{
	public:
		MGameInfo();
		~MGameInfo();
		bool Init();

		int		GetGameVersion() const	{ return nGameVersion; }
		const char*	GetGameName() const		{ return szGameName; }
		const char*	GetShaderPath() const	{ return szShaderPath; }
		const char*	GetContentPath() const	{ return szContentPath; }
		const char*	GetConfigPath() const	{ return szConfigPath; }

	private:
		int		nGameVersion;
		char	szGameName[256];
		char	szShaderPath[256];
		char	szContentPath[256];
		char	szConfigPath[256];
	};

	class MARKTECH_API MUserSettings
	{
	public:
		MUserSettings();
		~MUserSettings();
		bool Init(const MGameInfo& gameinfo);

		int GetWidth() { return nVSWidth; }
		int GetHeight() { return nVSHeight; }
		int GetWindowed() { return bVSWindowed; }
		int GetVSync() { return bVSVSync; }
	private:
		int nVSWidth;
		int nVSHeight;
		int bVSWindowed;
		int bVSVSync;
	};
}