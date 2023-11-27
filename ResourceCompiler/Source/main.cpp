#include <iostream>
#include <fstream>
#include <random>

#include "DirectXTex\DirectXTex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"

void WICToMTex(const char** argv)
{
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr;

	wchar_t* filepath = new wchar_t[strlen(argv[3]) + 1];

	if (argv[3] == nullptr)
		return;

	mbstowcs(filepath, argv[3], strlen(argv[3]) + 1);

	hr = DirectX::LoadFromWICFile(
		filepath,
		DirectX::WIC_FLAGS_DEFAULT_SRGB | DirectX::WIC_FLAGS_FILTER_LINEAR,
		&metadata, image);

	if (FAILED(hr))
	{
		std::cout << "Failed to find image\n";
		std::cin.get();
		return;
	}

	std::cout << "Loaded image!\n";

	DirectX::ScratchImage mipImage;

	int mipsAmount;

	if ((strcmp(argv[5], "-mips") == 0))
	{
		char* pchar;
		mipsAmount = strtol(argv[6], &pchar, 10);
	}
	else
	{
		mipsAmount = 3;
	}

	hr = DirectX::GenerateMipMaps(*image.GetImage(0, 0, 0), DirectX::TEX_FILTER_DEFAULT, mipsAmount, mipImage);

	if (FAILED(hr))
	{
		std::cout << "Failed to generate mipmaps!\n";
		std::cin.get();
		return;
	}

	std::cout << "Generated mipmaps!\n";

	DirectX::ScratchImage bcImage;

	hr = DirectX::Compress(mipImage.GetImages(), mipImage.GetImageCount(), mipImage.GetMetadata(), DXGI_FORMAT_BC3_UNORM_SRGB, DirectX::TEX_COMPRESS_SRGB_IN | DirectX::TEX_COMPRESS_DEFAULT, 0.5, bcImage);

	if (FAILED(hr))
	{
		std::cout << "Failed to compress image\n";
		std::cin.get();
		return;
	}

	std::cout << "Image compressed!\n";

	DirectX::Blob imgBlob;
	hr = DirectX::SaveToDDSMemory(mipImage.GetImages(), mipImage.GetImageCount(), mipImage.GetMetadata(), DirectX::DDS_FLAGS_NONE, imgBlob);

	std::fstream file;
	file.open(argv[4], std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "Couldn't find output directory\n";
		return;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t id = dist(rd);

	file.write((char*)imgBlob.GetBufferPointer(), imgBlob.GetBufferSize());
	file.close();

	std::cout << "Image exported!\n";

	return;
}

void MTexToMemory(const char* argv[])
{
	std::fstream file;
	file.open(argv[3], std::ios::in | std::ios::binary);

	if (!file.is_open())
		return;

	file.seekg(0, file.end);
	int length = file.tellg();
	char* pImg = new char[length];
	file.seekg(0, file.beg);

	file.read((char*)pImg, length);
	file.close();

	DirectX::ScratchImage scImg;
	DirectX::TexMetadata metadata;

	DirectX::LoadFromDDSMemory(pImg, length, DirectX::DDS_FLAGS_NONE, &metadata, scImg);
}

int main(int argc, const char *argv[])
{
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	int str = strcmp(argv[1], "-mtex");
	if (str == 0)
	{
		str = strcmp(argv[2], "-write");
		if (str == 0)
		{
			WICToMTex(argv);
			return 0;
		}

		str = strcmp(argv[2], "-read");
		if (str == 0)
		{
			MTexToMemory(argv);
		}
	}

	return 0;
}