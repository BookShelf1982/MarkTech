#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	enum class TextureFormat
	{
		R8G8B8A8,
		R8G8B8
	};

	struct Texture2D
	{
		U32 imageWidth;
		U32 imageHeight;
		TextureFormat imageFormat;
		char* pData;
		U64 dataLength;
	};

	Texture2D ReadTexture2DFile(const char* pFilepath);
}