#include "MModel.h"
#include <fstream>

namespace MarkTech
{
    CModel::CModel()
        :CAssetObject::CAssetObject(), m_nIndsAmount(0), m_nVertsAmount(0), m_pInds(), m_pVerts()
    {
	}
    CModel::~CModel()
    {
    }

    void CModel::Release()
    {
        m_pVerts.Release();
        m_pInds.Release();
    }

    CModel* LoadModel(const char* filepath)
    {
        CModel* tempAsset = new CModel();
        std::fstream file;
        file.open(filepath, std::ios::in | std::ios::binary);
        if (!file.is_open())
            return tempAsset;

        file.read((char*)&tempAsset->m_nAssetId, sizeof(uint64_t));
        file.read((char*)&tempAsset->m_nVertsAmount, sizeof(size_t));
        file.read((char*)&tempAsset->m_nIndsAmount, sizeof(size_t));
        tempAsset->m_pVerts.Initialize(tempAsset->m_nVertsAmount * sizeof(MVertex));
        tempAsset->m_pInds.Initialize(tempAsset->m_nIndsAmount * sizeof(uint32_t));
        file.read((char*)tempAsset->m_pVerts.GetPtr(), sizeof(MVertex) * tempAsset->m_nVertsAmount);
        file.read((char*)tempAsset->m_pInds.GetPtr(), sizeof(uint32_t) * tempAsset->m_nIndsAmount);
        file.close();

        return tempAsset;
    }
}