#include "File.h"

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	File::File(const char* pFilepath, FileAccessType accessType)
		:m_AccessType(accessType)
	{
		DWORD accessTypeArg = 0;
		switch (accessType)
		{
		case FileAccessType::READ:
			accessTypeArg = GENERIC_READ;
			break;
		case FileAccessType::WRITE:
			accessTypeArg = GENERIC_WRITE;
			break;
		case FileAccessType::READ_WRITE:
			accessTypeArg = GENERIC_READ | GENERIC_WRITE;
			break;
		}

		m_hFileHandle = CreateFileA(
			pFilepath,
			accessTypeArg,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (m_hFileHandle == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
			m_IsOpened = false;
		else
			m_IsOpened = true;
	}

	File::~File()
	{
		if (m_IsOpened)
			CloseHandle(m_hFileHandle);
	}

	void File::Close()
	{
		m_IsOpened = false;
		CloseHandle(m_hFileHandle);
	}

	void File::Read(char* pBuffer, U32 sizeOfBytes)
	{
		if (m_AccessType == FileAccessType::WRITE && m_IsOpened == true)
			return;

		ReadFile(
			m_hFileHandle,
			pBuffer,
			sizeOfBytes,
			NULL,
			NULL
		);
	}

	void File::Write(char* pBuffer, U32 sizeOfBytes)
	{
		if (m_AccessType == FileAccessType::READ && m_IsOpened == true)
			return;

		WriteFile(
			m_hFileHandle,
			pBuffer,
			sizeOfBytes,
			NULL,
			NULL
		);
	}
#endif
}