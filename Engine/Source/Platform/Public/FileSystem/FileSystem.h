#pragma once

struct File
{
	void* _handle;
	size_t size;
};

typedef void* FileIterator;

enum OpenFileType
{
	OPEN_TYPE_READ,
	OPEN_TYPE_WRITE
};

enum SeekOrigin
{
	SEEK_ORIGIN_START,
	SEEK_ORIGIN_CURRENT,
	SEEK_ORIGIN_END
};

struct FileInfo
{
	char* name;
};

typedef File (*PFN_FSOpen)(const char* filepath, OpenFileType openType);
typedef size_t (*PFN_FSRead)(File* file, char* buffer, size_t size);
typedef void (*PFN_FSWrite)(File* file, char* buffer, size_t size);
typedef void (*PFN_FSSeek)(File* file, size_t offset, SeekOrigin origin);
typedef size_t (*PFN_FSTell)(File* file);
typedef void (*PFN_FSClose)(File* file);
typedef bool (*PFN_FSDelete)(const char* filepath);
typedef bool (*PFN_FSDeleteDirectory)(const char* directory);

typedef FileIterator (*PFN_FSBeginFileIt)(const char* directory, FileInfo* info);
typedef bool (*PFN_FSFileIterate)(FileIterator it, FileInfo* info);
typedef void (*PFN_FSEndFileIt)(FileIterator it);

#ifndef FS_DLL
extern PFN_FSOpen FSOpen;
extern PFN_FSRead FSRead;
extern PFN_FSWrite FSWrite;
extern PFN_FSClose FSClose;
extern PFN_FSDelete FSDelete;
extern PFN_FSDeleteDirectory FSDeleteDirectory;
extern PFN_FSBeginFileIt FSBeginFileIt;
extern PFN_FSFileIterate FSFileIterate;
extern PFN_FSEndFileIt FSEndFileIt;
#endif