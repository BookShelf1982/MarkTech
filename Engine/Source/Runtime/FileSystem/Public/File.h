#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum class FileAccessType
	{
		READ,
		WRITE,
		READ_WRITE
	};

	class File
	{
	public:
		File(const char* pFilepath, FileAccessType accessType);
		~File();

		bool IsOpen() { return m_IsOpened; }
		void Close();
		void Read(char* pBuffer, U32 sizeOfBytes);
		void Write(char* pBuffer, U32 sizeOfBytes);
	private:
#ifdef MT_PLATFORM_WINDOWS
		HANDLE m_hFileHandle;
#endif
		bool m_IsOpened;
		FileAccessType m_AccessType;
	};
}