#include "Compiler.h"
#include <FileSystem\FileSystem.h>
#include "StringHash.h"

#include <xml.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct SpriteData
{
	uint32_t width, height;
	uint8_t* data;
	size_t dataSize;
};

bool CompileSprite(xml_node* asset, const char* name, const char* outputDir)
{
	size_t outputDirLen = strlen(outputDir);
	size_t nameLen = strlen(name);
	size_t outputPathLen = outputDirLen + nameLen + 5;
	char* outputPath = (char*)malloc(outputPathLen);
	strcpy_s(outputPath, outputPathLen, outputDir);
	strcat_s(outputPath, outputPathLen, name);
	strcat_s(outputPath, outputPathLen, ".spr");

	File file = FSOpen(outputPath, OPEN_TYPE_WRITE);
	free(outputPath);
	if (!file._handle)
		return false;

	SpriteData data = {};

	for (size_t i = 0; i < xml_node_children(asset); i++)
	{
		xml_node* child = xml_node_child(asset, i);
		xml_string* nodeName = xml_node_name(child);
		char name[32] = "";
		xml_string_copy(nodeName, (uint8_t*)name, sizeof(name));
		switch (hashString(name))
		{
		case "src_path"_sid:
		{
			xml_string* path = xml_node_content(child);
			size_t pathLength = xml_string_length(path);
			char* cpath = (char*)malloc(pathLength + 1);
			cpath[pathLength] = 0;
			xml_string_copy(path, (uint8_t*)cpath, pathLength);
			int width, height, channels = 0;
			data.data = stbi_load(cpath, &width, &height, &channels, 0);
			data.dataSize = width * height * channels;
		}
		break;
		case "width"_sid:
		{
			xml_string* width = xml_node_attribute_content(child, 0);
			char num[8] = "";
			xml_string_copy(width, (uint8_t*)num, sizeof(num));
			data.width = (uint32_t)atoi(num);
		}
		break;
		case "height"_sid:
		{
			xml_string* height = xml_node_attribute_content(child, 0);
			char num[8] = "";
			xml_string_copy(height, (uint8_t*)num, sizeof(num));
			data.height = (uint32_t)atoi(num);
		}
		break;
		}
	}

	FSWrite(&file, (char*)&data.width, sizeof(uint32_t));
	FSWrite(&file, (char*)&data.height, sizeof(uint32_t));
	FSWrite(&file, (char*)data.data, data.dataSize);
	FSClose(&file);

	stbi_image_free(data.data);
	printf("Succesfully compiled asset: %s", name);

	return true;
}

bool GenericCompile(const char* filepath, const char* outputDir)
{
	File file = FSOpen(filepath, OPEN_TYPE_READ);
	uint8_t* data = (uint8_t*)malloc(file.size);
	FSRead(&file, (char*)data, file.size);
	FSClose(&file);
	xml_document* doc = xml_parse_document(data, file.size);
	if (!doc)
		return false;
	xml_node* root = xml_document_root(doc);
	xml_string* name = xml_node_attribute_name(root, 0);
	xml_string* content = xml_node_attribute_content(root, 0);
	char buffer[8] = "";
	xml_string_copy(name, (uint8_t*)buffer, 8);
	if (hashString(buffer) != "type"_sid)
		return false;

	xml_string_copy(content, (uint8_t*)buffer, sizeof(buffer));

	switch (hashString(buffer))
	{
	case "sprite"_sid:
	{
		size_t start = 0;
		size_t end = 0;
		for (size_t i = strlen(filepath); i > 0; i--)
		{
			if (filepath[i] == '.')
			{
				end = i;
			}
			if (filepath[i] == '\\')
			{
				start = i + 1;
				break;
			}
		}

		size_t len = (end - start);
		char* assetName = (char*)malloc(len + 1);
		assetName[len] = 0;
		memcpy_s(assetName, len, filepath + start, len);
		CompileSprite(root, assetName, outputDir);
		free(assetName);
	} break;
	}
	
	xml_document_free(doc, true);
	return true;
}