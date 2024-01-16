#pragma once
#include "Core.h"
#include "AssetRegistry.h"

struct MModelComponent
{
public:
	uint64_t nId;
	uint64_t nOwnerId;
	MModelAsset* pModelAsset;
};