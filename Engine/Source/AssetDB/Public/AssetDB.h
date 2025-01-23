/*
* 
* Asset pak (mpk) layout
* 
* assetEntriesLength
* assetDataBlobLength
* assetEntries[assetEntriesLength]
*	- id (first 8 bits is type and the 24 bits after is id)
*	- size
*	- offsetToData
* assetDataBlob[assetDataBlobLength]
* 
*/

/*
* 
* Base Asset Entry layout (.mae file)
* 
* assetType
* assetId
* nameLength
* name[nameLength]
* pathLength
* path[pathLength]
* 
*/

#include <stdint.h>

enum AssetType
{
	ASSET_TYPE_SPRITE
};

struct AssetEntryInfo
{
	AssetType type;
	uint32_t id;
	char* name;
	char* path;
	char* outputPath;
};

void WriteAssetEntryFile(const AssetEntryInfo* info);