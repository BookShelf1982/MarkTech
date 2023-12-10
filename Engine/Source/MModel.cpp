#include "MModel.h"
#include <fstream>

namespace MarkTech
{
    CModel::CModel()
    {
	}
    CModel::~CModel()
    {
    }

    CAssetObject* LoadModel(const char* filepath)
    {
        return new CModel();
    }
}