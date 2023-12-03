#include "Shader.h"
#include <fstream>

namespace MarkTech
{
	void CShader::Initialize(const void* bytecode, size_t size)
	{
		m_pShaderByteCode = bytecode;
		m_nShaderByteCodeSize = size;
	}

	MAssetData LoadShader(const char* filepath)
	{
		MAssetData TempAssetData = {};

		std::fstream file;
		file.open(filepath, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return TempAssetData;

		size_t nSourcelength = 0;

		file.read((char*)&TempAssetData.nId, sizeof(uint64_t));
		file.read((char*)&nSourcelength, sizeof(size_t));
		file.read((char*)&TempAssetData.nDataSize, sizeof(size_t));
		TempAssetData.pData = new char[TempAssetData.nDataSize];
		file.seekg(nSourcelength, std::ios::cur);
		file.read((char*)TempAssetData.pData, TempAssetData.nDataSize);
		file.close();

		return TempAssetData;
	}
}