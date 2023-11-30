#include <iostream>
#include <fstream>
#include <random>

#include "DirectXTex\DirectXTex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"

void WICToMTex(const char** argv)
{
	DirectX::ScratchImage* currentImage;
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr;

	wchar_t* filepath = new wchar_t[strlen(argv[2]) + 1];

	if (argv[2] == nullptr)
		return;

	mbstowcs(filepath, argv[2], strlen(argv[2]) + 1);

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

	currentImage = &image;

	DirectX::ScratchImage mipImage;

	int mipsAmount;

	if ((strcmp(argv[4], "-nomips") != 0))
	{
		if ((strcmp(argv[4], "-mips") == 0))
		{
			char* pchar;
			mipsAmount = strtol(argv[5], &pchar, 10);
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

		currentImage = &mipImage;
	}

	DirectX::ScratchImage bcImage;
	DirectX::Blob imgBlob;

	if (argv[6] != nullptr)
	{
		if ((strcmp(argv[6], "-compress") == 0))
		{
			hr = DirectX::Compress(currentImage->GetImages(), currentImage->GetImageCount(), currentImage->GetMetadata(), DXGI_FORMAT_BC1_UNORM_SRGB, DirectX::TEX_COMPRESS_SRGB_IN | DirectX::TEX_COMPRESS_DEFAULT, 0.5, bcImage);

			if (FAILED(hr))
			{
				std::cout << "Failed to compress image\n";
				std::cin.get();
				return;
			}

			std::cout << "Image compressed!\n";

			currentImage = &bcImage;
		}
	}

	hr = DirectX::SaveToDDSMemory(currentImage->GetImages(), currentImage->GetImageCount(), currentImage->GetMetadata(), DirectX::DDS_FLAGS_NONE, imgBlob);

	std::fstream file;
	file.open(argv[3], std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "Couldn't find output directory\n";
		return;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t id = dist(rd);

	std::cout << "Asset id: " << id << "\n";

	file.write((char*)&id, sizeof(id));
	file.write((char*)imgBlob.GetBufferPointer(), imgBlob.GetBufferSize());
	file.close();

	std::cout << "Image exported!\n";

	return;
}

int main(int argc, const char *argv[])
{
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	int str = strcmp(argv[1], "-mtex");
	if (str == 0)
	{
		WICToMTex(argv);
		return 0;
	}

	return 0;
}