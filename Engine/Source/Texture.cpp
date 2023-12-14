#include "Texture.h"
#include <fstream>

namespace MarkTech
{
	CTexture::CTexture()
		:CAssetObject::CAssetObject(), m_nImgDataSize(0), m_pImgData()
	{
	}

	CTexture::~CTexture()
	{
		OutputDebugStringA("Deleted texture!");
	}

	void CTexture::Release()
	{
		m_pImgData.Release();
		delete this;
	}

	CTexture* LoadTexture(const char* filepath)
	{
		CTexture* tempAsset = new CTexture();
		std::fstream file;
		file.open(filepath, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return tempAsset;

		file.read((char*)&tempAsset->m_nAssetId, sizeof(uint64_t));
		file.seekg(sizeof(uint64_t), file.end);
		std::streampos length = file.tellg();
		tempAsset->m_nImgDataSize = length;
		tempAsset->m_pImgData.Initialize(tempAsset->m_nImgDataSize);
		file.seekg(sizeof(uint64_t), file.beg);
		file.read((char*)tempAsset->m_pImgData.GetPtr(), length);
		file.close();

		return tempAsset;
	}
}