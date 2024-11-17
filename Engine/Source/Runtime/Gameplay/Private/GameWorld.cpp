#include "GameWorld.h"
#include <ResourceManager.h>
#include <Log.h>

#include <random>
#include <string.h>

namespace MarkTech
{
	void CreateGameWorld(GameWorld& world, U32 maxGameElements)
	{
		world.maxGameElements = maxGameElements;
		void* pAllocMem = malloc((sizeof(U32) * world.maxGameElements));
		world.pEntities = (U32*)pAllocMem;
	}

	void UpdateSprite(SpriteComponent& sprite)
	{
		if (sprite.rotation >= 360.0f)
		{
			sprite.rotation = 0.0f;
			return;
		}
		
		sprite.rotation += 5.0f;
	}

	void TickGameWorld(GameWorld& world)
	{
	}

	void DestroyGameWorld(GameWorld& world)
	{
		free(world.pEntities);
	}

	U32 CreateEntity(GameWorld& world)
	{
		std::random_device rd;
		std::uniform_int_distribution<U32> dist;
		world.pEntities[world.enabledEntities] = dist(rd);
		world.enabledEntities++;
		return world.pEntities[world.enabledEntities - 1];
	}

	void DestroyEntity(GameWorld& world, U32 id)
	{
		for (U32 i = 0; i < world.enabledEntities; i++)
		{
			if (world.pEntities[i] == id)
			{
				world.pEntities[i] = world.pEntities[world.enabledEntities - 1];
				world.enabledEntities--;
			}
		}
	}
}