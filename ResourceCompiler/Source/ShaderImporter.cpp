#include "ShaderImporter.h"
#include <iostream>
#include <fstream>
#include <random>

int LoadShader(const char* filepath, const char* output, const char* shadertype, const char* entrypoint)
{
	std::fstream InputFile;
	InputFile.open(filepath, std::ios::in);
	if (!InputFile.is_open())
	{
		std::cout << "Failed to find shader.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	InputFile.seekg(0, std::ios::end);
	size_t nSourceLength = InputFile.tellg();
	InputFile.seekg(0, std::ios::beg);
	const char* pszSourceFile = new char[nSourceLength];
	InputFile.read((char*)pszSourceFile, nSourceLength);
	InputFile.close();

	HRESULT hr;
	ID3D10Blob* pShaderByteCode;
	ID3D10Blob* pError;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	wchar_t* wstr = new wchar_t[strlen(filepath) + 1];
	mbstowcs(wstr, filepath, strlen(filepath) + 1);

	hr = D3DCompileFromFile(wstr, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint, shadertype, flags, NULL, &pShaderByteCode, &pError);
	if (FAILED(hr))
	{
		if (pError != nullptr)
		{
			std::cout << (char*)pError->GetBufferPointer() << "\n";
			std::cout << "Failed to compile shader.\nPress enter to exit...\n";
			std::cin.get();
			pError->Release();
			if (pShaderByteCode != nullptr)
				pShaderByteCode->Release();

			return -1;
		}
	}

	std::cout << "Shader compile succeded!\n";

	std::string Path = output;
	std::fstream model;
	// If the file is not an .mmdl file return false
	if (Path.substr(Path.size() - 4, 4) != ".mfx")
	{
		std::cout << "Output file is not an mfx file.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	std::fstream OutputFile;
	OutputFile.open(output, std::ios::out | std::ios::binary);

	if (!OutputFile.is_open())
	{
		std::cout << "Failed to find output path.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t assetId = dist(rd);

	std::cout << "Asset Id: " << assetId << "\n";
	size_t bytecodesize = pShaderByteCode->GetBufferSize();
	OutputFile.write((char*)&assetId, sizeof(uint64_t));
	OutputFile.write((char*)&nSourceLength, sizeof(size_t));
	OutputFile.write((char*)&bytecodesize, sizeof(size_t));
	OutputFile.write((char*)pszSourceFile, nSourceLength);
	OutputFile.write((char*)pShaderByteCode->GetBufferPointer(), pShaderByteCode->GetBufferSize());
	OutputFile.close();

	pShaderByteCode->Release();
	delete[] pszSourceFile;

	std::cout << "Shader exported!\n";

	return 0;
}
