#pragma once
#include "Core.h"
#include "inipp.h"
#include <fstream>


struct MGameInfo
{
public:
	static MGameInfo& GetGameInfo()
	{
		return m_pGInfo;
	}

	bool Init();

	int gameversion;
	std::string GameName;
	std::string ShaderPath;
	std::string RawShaderPath;

private:
	MGameInfo() :gameversion(0), GameName(""), ShaderPath(""), RawShaderPath("") {}

	static MGameInfo m_pGInfo;
};

