#pragma once
#include <Math3D.h>
#include <PrimitiveTypes.h>
#include <NBT.h>

namespace MarkTech
{
#define MT_MAX_ENTITIES 2048

	struct SpriteComponent
	{
		U32 id;
		U32 ownerId;
		U32 resourceId;
		Vector3 origin;
		F32 scale;
		F32 rotation;
	};

	struct NBTDataComponent
	{
		U32 id;
		U32 ownerId;
		NBTStructure nbt;
	};

	struct GameWorld
	{
		U32 maxGameElements;
		U32* pEntities;
		U32 enabledEntities;
		NBTDataComponent* pNBTs;
		U32 enabledNBTs;
	};

	void CreateGameWorld(GameWorld& world, U32 maxGameElements);
	void TickGameWorld(GameWorld& world);
	void DestroyGameWorld(GameWorld& world);

	U32 CreateEntity(GameWorld& world);
	void DestroyEntity(GameWorld& world, U32 id);
}