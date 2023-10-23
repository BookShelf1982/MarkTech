#include "UserSettings.h"

bool MUserSettings::Init()
{
	std::ifstream in("UserSettings.ini");
	inipp::Ini<char> ini;
	if (in.is_open())
	{
		ini.parse(in);
		inipp::get_value(ini.sections["MarkTech.VideoSettings"], "VideoSettings.Width", m_UserSettings.nVSWidth);
		inipp::get_value(ini.sections["MarkTech.VideoSettings"], "VideoSettings.Height", m_UserSettings.nVSLength);
		inipp::get_value(ini.sections["MarkTech.VideoSettings"], "VideoSettings.Windowed", m_UserSettings.bVSWindowed);
		inipp::get_value(ini.sections["MarkTech.VideoSettings"], "VideoSettings.VSync", m_UserSettings.bVSVSync);
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

MUserSettings MUserSettings::m_UserSettings;