#include "Shader.h"
#include <fstream>

namespace MarkTech
{
	CShader::CShader()
		: CAssetObject::CAssetObject(), m_nShaderByteCodeSize(0), m_pShaderByteCode()
	{
	}
	CShader::~CShader()
	{
		OutputDebugStringA("Deleted Shader!\n");
	}

	void CShader::Release()
	{
		char str[256];
		sprintf_s(str, sizeof(str), "Deleted Memory Block of: %i bytes! \n", (int)m_nShaderByteCodeSize);
		OutputDebugStringA(str);
		m_pShaderByteCode.Release();
		delete this;
	}

	CAssetObject* LoadShader(const char* filepath)
	{
		CShader* shader = new CShader();

		uint64_t nTempId = 0;
		size_t nSourceSize = 0;
		size_t nBytecodeSize = 0;


		std::fstream file;
		file.open(filepath, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return nullptr;

		file.read((char*)&shader->m_nAssetId, sizeof(uint64_t));	//Get Asset Id
		file.read((char*)&nSourceSize, sizeof(size_t));				//Get Shader source size
		file.read((char*)&nBytecodeSize, sizeof(size_t));			//Get Shader bytecode size
		file.seekg(nSourceSize, std::ios::cur);						//Skip shader source data
		shader->m_pShaderByteCode.Initialize(nBytecodeSize);
		file.read((char*)shader->m_pShaderByteCode.GetPtr(), nBytecodeSize);	//Get Shader bytecode
		file.close();												//Close file

		shader->m_nShaderByteCodeSize = nBytecodeSize;

		return shader;
	}
}