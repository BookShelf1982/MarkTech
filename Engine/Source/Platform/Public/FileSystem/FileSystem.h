#pragma once

#ifdef MT_PLATFORM_WINDOWS
typedef void* File;
#endif
enum OpenFileType
{
	OPEN_TYPE_READ = 0x1,
	OPEN_TYPE_WRITE = 0x2
};

typedef File(*PFN_FSOpen)(const char* filepath, OpenFileType openType);
typedef void (*PFN_FSClose)(File file);