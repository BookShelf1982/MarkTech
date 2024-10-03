#pragma once
#include <Math3D.h>
#include <PrimitiveTypes.h>

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

	struct GameWorld
	{
		U32 maxGameElements;
		U32* pEntities;
		U32 enabledEntities;
		SpriteComponent* pSprites;
		U32 enabledSprites;
	};

	GameWorld CreateGameWorld(U32 maxGameElements);
	void TickGameWorld(GameWorld* pWorld);
	void DestroyGameWorld(GameWorld* pWorld);

	U32 CreateEntity(GameWorld* pWorld);
	void DestroyEntity(GameWorld* pWorld, U32 id);
}