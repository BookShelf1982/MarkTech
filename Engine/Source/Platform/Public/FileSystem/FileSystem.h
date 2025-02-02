#pragma once

struct PathRange
{
	size_t offset;
	size_t length;
};

#define MAX_PATH_LEN 260

struct Path
{
	char path[MAX_PATH_LEN];
};

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

typedef Path (*PFN_FSCreatePath)(const char* path);
typedef void (*PFN_FSRemoveTrailing)(Path* path);
typedef void (*PFN_FSAddBackslash)(Path* path);
typedef void (*PFN_FSAddExtension)(Path* path, const char* extension);
typedef void (*PFN_FSRenameExtension)(Path* path, const char* extension);
typedef void (*PFN_FSAppend)(Path* path, const char* str);
typedef const char* (*PFN_FSFindExtension)(Path* path);
typedef const char* (*PFN_FSFindFilename)(Path* path);
typedef bool (*PFN_FSIsDirectory)(Path* path);


typedef File (*PFN_FSOpen)(const char* filepath, OpenFileType openType);
typedef File (*PFN_FSOpenWithPath)(Path* filepath, OpenFileType openType);
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
extern PFN_FSCreatePath FSCreatePath;
extern PFN_FSRemoveTrailing FSRemoveTrailing;
extern PFN_FSAddExtension FSAddExtension;
extern PFN_FSRenameExtension FSRenameExtension;
extern PFN_FSAppend FSAppend;
extern PFN_FSFindExtension FSFindExtension;
extern PFN_FSFindFilename FSFindFilename;
extern PFN_FSIsDirectory FSIsDirectory;
extern PFN_FSOpen FSOpen;
extern PFN_FSOpenWithPath FSOpenWithPath;
extern PFN_FSRead FSRead;
extern PFN_FSWrite FSWrite;
extern PFN_FSClose FSClose;
extern PFN_FSDelete FSDelete;
extern PFN_FSDeleteDirectory FSDeleteDirectory;
extern PFN_FSBeginFileIt FSBeginFileIt;
extern PFN_FSFileIterate FSFileIterate;
extern PFN_FSEndFileIt FSEndFileIt;
#endif