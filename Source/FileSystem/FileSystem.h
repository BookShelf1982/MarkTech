#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_PATH_LEN 260

typedef struct {
	char path[MAX_PATH_LEN];
} Path;

typedef struct {
	void* _handle;
	size_t size;
} File;

//typedef void* FileIterator;

typedef enum {
	OPEN_TYPE_READ,
	OPEN_TYPE_WRITE
} OpenFileType;

typedef enum {
	SEEK_ORIGIN_START,
	SEEK_ORIGIN_CURRENT,
	SEEK_ORIGIN_END
} SeekOrigin;

/*struct FileInfo
{
	char name[MAX_PATH_LEN];
};*/

void FSAddBackslash(Path* path);
void FSAddExtension(Path* path, const char* ext);
void FSAppend(Path* path, const char* str);
char* FSFindExtension(Path* path);
char* FSFindFilename(Path* path);
unsigned char FSIsDirectory(Path* path);
unsigned char FSPathExists(Path* path);
void FSRemoveTrailing(Path* path);
void FSGetAbsolutePath(Path* path);
void FSRenameExtension(Path* path, const char* extension);

void FSOpen(Path* path, OpenFileType openType, File* file);
size_t FSRead(File* file, char* buffer, size_t size);
void FSWrite(File* file, char* buffer, size_t size);
void FSSeek(File* file, size_t offset, SeekOrigin origin);
void FSClose(File* file);

unsigned char FSDelete(const char* filepath);
unsigned char FSDeleteDirectory(const char* directory);

/*FileIterator FSBeginFileIt(const char* directory, FileInfo* info);
unsigned char FSFileIterate(FileIterator it, FileInfo* info);
void FSEndIt(FileIterator it);*/
#endif