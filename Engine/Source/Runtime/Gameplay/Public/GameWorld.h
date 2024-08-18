#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
#define MT_MAX_ENTITIES 2048

	struct MessageComponent
	{
		U32 id;
		U32 ownerId;
		U32 resoruceId;
	};

	struct GameWorld
	{
		U32 maxGameElements;
		U32* pEntities;
		U32 enabledEntities;
	};

	GameWorld CreateGameWorld(U32 maxGameElements);
	void DestroyGameWorld(GameWorld* pWorld);

	U32 CreateEntity(GameWorld* pWorld);
	void DestroyEntity(GameWorld* pWorld, U32 id);
}