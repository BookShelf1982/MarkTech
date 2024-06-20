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

	enum class SeekOrigin
	{
		BEGINING,
		CURRENT,
		END
	};

	class File
	{
	public:
		File(const char* pFilepath, FileAccessType accessType);
		~File();

		U64 GetSize() const;
		bool IsOpen() const { return m_IsOpened; }

		void Close();
		void Read(char* pBuffer, U32 sizeOfBytes);
		void Write(char* pBuffer, U32 sizeOfBytes);
		void Seek(U64 location, SeekOrigin orign);
	private:
#ifdef MT_PLATFORM_WINDOWS
		HANDLE m_hFileHandle;
#endif
		bool m_IsOpened;
		FileAccessType m_AccessType;
		U64 m_Size;
	};
}