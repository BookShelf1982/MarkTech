#include "File.h"

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	File::File(const char* pFilepath, FileAccessType accessType)
		:m_AccessType(accessType)
	{
		DWORD accessTypeArg = 0;
		DWORD openMode = 0;
		switch (accessType)
		{
		case FileAccessType::READ:
			accessTypeArg = GENERIC_READ;
			openMode = OPEN_EXISTING;
			break;
		case FileAccessType::WRITE:
			accessTypeArg = GENERIC_WRITE;
			openMode = CREATE_NEW;
			break;
		case FileAccessType::READ_WRITE:
			accessTypeArg = GENERIC_READ | GENERIC_WRITE;
			openMode = OPEN_ALWAYS;
			break;
		}

		m_hFileHandle = CreateFileA(
			pFilepath,
			accessTypeArg,
			0,
			NULL,
			openMode,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (m_hFileHandle == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND)
			m_IsOpened = false;
		else
			m_IsOpened = true;
		
		LARGE_INTEGER size = {};
		if (!GetFileSizeEx(m_hFileHandle, &size))
			DWORD err = GetLastError();

		m_Size = size.QuadPart;
	}

	File::~File()
	{
		if (m_IsOpened)
			CloseHandle(m_hFileHandle);
	}

	U64 File::GetSize() const
	{
		return m_Size;
	}

	void File::Close()
	{
		m_IsOpened = false;
		CloseHandle(m_hFileHandle);
	}

	void File::Read(char* pBuffer, U64 sizeOfBytes)
	{
		if (m_AccessType == FileAccessType::WRITE || m_IsOpened == false)
			return;

		ReadFile(
			m_hFileHandle,
			pBuffer,
			(U32)sizeOfBytes,
			NULL,
			NULL
		);
	}

	void File::Write(char* pBuffer, U64 sizeOfBytes)
	{
		if (m_AccessType == FileAccessType::READ || m_IsOpened == false)
			return;

		WriteFile(
			m_hFileHandle,
			pBuffer,
			(U32)sizeOfBytes,
			NULL,
			NULL
		);
	}

	void File::Seek(I32 location, SeekOrigin orign)
	{
		if (m_AccessType == FileAccessType::WRITE || m_IsOpened == false)
			return;

		DWORD moveMethod = FILE_CURRENT;

		switch (orign)
		{
		case MarkTech::SeekOrigin::BEGINING:
			moveMethod = FILE_BEGIN;
			break;
		case MarkTech::SeekOrigin::CURRENT:
			moveMethod = FILE_CURRENT;
			break;
		case MarkTech::SeekOrigin::END:
			moveMethod = FILE_END;
			break;
		default:
			moveMethod = FILE_CURRENT;
			break;
		}

		SetFilePointer(
			m_hFileHandle,
			location,
			NULL,
			moveMethod
		);
	}
#endif
}