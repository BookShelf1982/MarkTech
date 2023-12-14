#include "TextureImporter.h"
#include <iostream>
#include <fstream>
#include <random>

int LoadTexture(const char* filepath, const char* compiledname, int mips, int compression)
{
	DirectX::ScratchImage* currentImage;
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr;

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		std::cout << "Failed to CoInitialize\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	wchar_t* wstr = new wchar_t[strlen(filepath) + 1];

	if (filepath == nullptr)
		return -1;

	mbstowcs(wstr, filepath, strlen(filepath) + 1);

	hr = DirectX::LoadFromWICFile(
		wstr,
		DirectX::WIC_FLAGS_DEFAULT_SRGB | DirectX::WIC_FLAGS_FILTER_LINEAR,
		&metadata, image);

	if (FAILED(hr))
	{
		std::cout << "Failed to find image!\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	std::cout << "Loaded image!\n";

	currentImage = &image;

	DirectX::ScratchImage mipImage;

	if (mips > 1)
	{
		hr = DirectX::GenerateMipMaps(*image.GetImage(0, 0, 0), DirectX::TEX_FILTER_DEFAULT, mips, mipImage);

		if (FAILED(hr))
		{
			std::cout << "Failed to generate mipmaps!\nPress enter to exit...\n";
			std::cin.get();
			return -1;
		}

		std::cout << "Generated mipmaps!\n";

		currentImage = &mipImage;
	}

	DirectX::ScratchImage bcImage;

	if (compression >= 0)
	{
		if (compression == 0)
			hr = DirectX::Compress(currentImage->GetImages(), currentImage->GetImageCount(), currentImage->GetMetadata(), DXGI_FORMAT_BC1_UNORM_SRGB, DirectX::TEX_COMPRESS_SRGB_IN | DirectX::TEX_COMPRESS_DEFAULT, 0.5, bcImage);
		else if (compression == 1)
			hr = DirectX::Compress(currentImage->GetImages(), currentImage->GetImageCount(), currentImage->GetMetadata(), DXGI_FORMAT_BC5_UNORM, DirectX::TEX_COMPRESS_SRGB_IN | DirectX::TEX_COMPRESS_DEFAULT, 0.5, bcImage);
		else
		{
			hr = E_FAIL;
		}

		if (FAILED(hr))
		{
			std::cout << "Failed to compress image\nPress enter to exit...\n";
			std::cin.get();
			return -1;
		}

		std::cout << "Image compressed!\n";

		currentImage = &bcImage;
	}

	std::string Path = compiledname;
	std::fstream model;
	// If the file is not an .mmdl file return false
	if (Path.substr(Path.size() - 5, 5) != ".mtex")
	{
		std::cout << "Output file is not an mtex file.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	std::fstream exportfile;
	exportfile.open(compiledname, std::ios::out | std::ios::binary);

	if (!exportfile.is_open())
	{
		std::cout << "Failed to find image export output!\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}
	DirectX::Blob imgBlob;
	DirectX::SaveToDDSMemory(currentImage->GetImages(), currentImage->GetImageCount(), currentImage->GetMetadata(), DirectX::DDS_FLAGS_NONE, imgBlob);

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t assetId = dist(rd);
	std::cout << "Asset Id: " << assetId << "\n";


	exportfile.write((char*)&assetId, sizeof(uint64_t));
	exportfile.write((char*)imgBlob.GetBufferPointer(), imgBlob.GetBufferSize());
	exportfile.close();

	std::cout << "Image Exported!\nPress enter to exit...\n";
	std::cin.get();

	return 0;
}
