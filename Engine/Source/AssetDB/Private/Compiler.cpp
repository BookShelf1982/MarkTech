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
	Path outputPath = FSCreatePath(outputDir);
	FSAppend(&outputPath, name);
	FSRenameExtension(&outputPath, ".spr");

	File file = FSOpenWithPath(&outputPath, OPEN_TYPE_WRITE);
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
			Path filepath = {};
			xml_string_copy(path, (uint8_t*)filepath.path, sizeof(filepath));
			int width, height, channels = 0;
			data.data = stbi_load(filepath.path, &width, &height, &channels, 0);
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
	if (!file._handle)
		return false;

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
		Path path = FSCreatePath(filepath);
		const char* assetName = FSFindFilename(&path);
		
		CompileSprite(root, assetName, outputDir);
	} break;
	}
	
	xml_document_free(doc, true);
	return true;
}