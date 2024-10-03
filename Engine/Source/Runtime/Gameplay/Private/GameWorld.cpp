#include "GameWorld.h"
#include <ResourceManager.h>
#include <Log.h>

#include <random>
#include <string.h>

namespace MarkTech
{
	GameWorld CreateGameWorld(U32 maxGameElements)
	{
		GameWorld world = {};
		world.maxGameElements = maxGameElements;
		void* pAllocMem = malloc((sizeof(U32) * world.maxGameElements) + (sizeof(SpriteComponent) * world.maxGameElements));
		world.pEntities = (U32*)pAllocMem;
		world.pSprites = (SpriteComponent*)((char*)pAllocMem + (sizeof(U32) * world.maxGameElements));
		return world;
	}

	void UpdateSprite(SpriteComponent* pSprite)
	{
		if (pSprite->rotation >= 360.0f)
		{
			pSprite->rotation = 0.0f;
			return;
		}
		
		pSprite->rotation += 5.0f;
	}

	void TickGameWorld(GameWorld* pWorld)
	{
		for (U32 i = 0; i < pWorld->enabledSprites; i++)
		{
			UpdateSprite(&pWorld->pSprites[i]);
		}
	}

	void DestroyGameWorld(GameWorld* pWorld)
	{
		free(pWorld->pEntities);
	}

	U32 CreateEntity(GameWorld* pWorld)
	{
		std::random_device rd;
		std::uniform_int_distribution<U32> dist;
		pWorld->pEntities[pWorld->enabledEntities] = dist(rd);
		pWorld->enabledEntities++;
		return pWorld->pEntities[pWorld->enabledEntities - 1];
	}

	void DestroyEntity(GameWorld* pWorld, U32 id)
	{
		for (U32 i = 0; i < pWorld->enabledEntities; i++)
		{
			if (pWorld->pEntities[i] == id)
			{
				pWorld->pEntities[i] = pWorld->pEntities[pWorld->enabledEntities - 1];
				pWorld->enabledEntities--;
			}
		}
	}
}