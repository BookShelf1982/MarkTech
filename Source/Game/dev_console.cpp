#include "dev_console.h"

#include <string.h>

namespace MarkTech
{
	/*
	float DevConsole::GetConVar(const char* name)
	{
		for (size_t i = 0; i < m_conVars.count; i++)
		{
			if (strcmp(name, m_conVars[i].name) == 0)
			{
				return m_conVars[i].val;
			}
		}

		ConVar var;
		strcpy_s(var.name, name);
		var.val = 0.0f;

		m_conVars.Insert(var);

		return var.val;
	}

	void DevConsole::SetConVar(const char* name, float val)
	{
		for (size_t i = 0; i < m_conVars.count; i++)
		{
			if (strcmp(name, m_conVars[i].name) == 0)
			{
				m_conVars[i].val = val;
				return;
			}
		}
	}

	void DevConsole::AddConFn(const char* name, PFN_ConFn fn)
	{
		ConFn conFn;
		conFn.fn = fn;
		strcpy_s(conFn.name, name);
		m_conFns.Insert(conFn);
	}

	void DevConsole::ExecConFn(const char* name)
	{
		for (size_t i = 0; i < m_conFns.count; i++)
		{
			if (strcmp(name, m_conFns[i].name) == 0)
			{
				m_conFns[i].fn();
				return;
			}
		}
	}
	*/
}