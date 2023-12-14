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
		MVertex(float x, float y, float z, float u, float v) : Position(x, y, z), TexCoords(u, v), Normal(0.0f, 0.0f, 0.0f) {}

		MVector3 Position;
		MVector3 Normal;
		MVector2 TexCoords;
	};

	MAKE_CTARRAY_COMPATIBLE(MVertex);

	class MARKTECH_API CModel : public CAssetObject
	{
	public:
		CModel();
		~CModel();

		virtual void Release() override;

		CTMemoryBlob<MVertex> m_pVerts;
		CTMemoryBlob<uint32_t> m_pInds;
		size_t m_nVertsAmount;
		size_t m_nIndsAmount;
	};

	MAKE_CTARRAY_COMPATIBLE(CModel);

	CModel* LoadModel(const char* filepath);
}