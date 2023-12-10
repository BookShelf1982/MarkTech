#pragma once
#include "Core.h"
#include "Vectors.h"
#include "AssetRegistry.h"
#include "Material.h"

namespace MarkTech
{
	struct MVertex
	{
		MVertex() {}
		MVertex(float x, float y, float z, float u, float v) : Position(x, y, z), TexCoords(u, v) {}

		MVector3 Position;
		MVector2 TexCoords;
	};

	MAKE_CTARRAY_COMPATIBLE(MVertex);

	class MARKTECH_API CModel : public CAssetObject
	{
	public:
		CModel();
		~CModel();

	protected:
		uint64_t assetId;
		MVertex* m_pVerts;
		uint32_t* m_pInds;
		size_t m_nVertsAmount;
		size_t m_nIndsAmount;
		CMaterial m_Material;
	};

	MAKE_CTARRAY_COMPATIBLE(CModel);

	CAssetObject* LoadModel(const char* filepath);
}