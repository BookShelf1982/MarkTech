#pragma once
#include "Core.h"
#include "Vectors.h"

struct MTransformComponent
{
	uint64_t nId;
	uint64_t nOwnerId;
	MTransform Transform;
};