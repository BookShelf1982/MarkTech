#include "GameInfo.h"

bool MGameInfo::Init()
{
	//std::ifstream in("C:/MarkTech/Build/GameInfo.ini");
	std::ifstream in("GameInfo.ini");
	inipp::Ini<char> ini;
	if (in.is_open())
	{
		ini.parse(in);
		inipp::get_value(ini.sections["MarkTech.GameInfo"], "Game.GameName", m_pGInfo.GameName);
		inipp::get_value(ini.sections["MarkTech.GameInfo"], "Game.GameVersion", m_pGInfo.gameversion);
		inipp::get_value(ini.sections["MarkTech.GameInfo"], "Game.ShaderPath", m_pGInfo.ShaderPath);
		inipp::get_value(ini.sections["MarkTech.GameInfo"], "Game.RawShaderPath", m_pGInfo.RawShaderPath);
		return true;
	}
	else
	{
		return false;
	}
}

MGameInfo MGameInfo::m_pGInfo = MGameInfo();

