#include "gameplay.h"
#include "alloc_uitls.h"

void RecordSnapshot(GameWorld& world , SceneSnapshot& snapshot)
{
	if (!snapshot.models.elements)
	{
		snapshot.models.elements = (uint32_t*)AlignedAlloc(sizeof(uint32_t) * MAX_OBJECTS, alignof(uint32_t));
		snapshot.models.capacity = MAX_OBJECTS;
		snapshot.models.size = 0;
	}

	if (!snapshot.origins.elements)
	{
		snapshot.origins.elements = (vec3f_t*)AlignedAlloc(sizeof(vec3f_t) * MAX_OBJECTS, alignof(vec3f_t));
		snapshot.origins.capacity = MAX_OBJECTS;
		snapshot.origins.size = 0;
	}

	for (size_t i = 0; i < world.meshes.count; i++)
	{
		if (snapshot.models.size + 1 > snapshot.models.capacity)
		{
			snapshot.models.capacity *= 2;
			snapshot.models.elements = (uint32_t*)AlignedRealloc(
				snapshot.models.elements,
				snapshot.models.capacity * sizeof(uint32_t),
				alignof(uint32_t)
			);
		}

		snapshot.models[snapshot.models.size] = world.meshes[i].meshHandle;
		snapshot.models.size++;

		if (snapshot.origins.size + 1 > snapshot.origins.capacity)
		{
			snapshot.origins.capacity *= 2;
			snapshot.origins.elements = (vec3f_t*)AlignedRealloc(
				snapshot.origins.elements,
				snapshot.origins.capacity * sizeof(vec3f_t),
				alignof(vec3f_t)
			);
		}

		snapshot.origins[snapshot.origins.size] = world.meshes[i].origin;
		snapshot.origins.size++;
	}
}

void TickGameWorld(GameWorld& world, float dt)
{
	RecordSnapshot(world, world.snapshot);
}

uint32_t CreateMeshObject(GameWorld& world)
{
	world.meshes.count++;
	return (uint32_t)world.meshes.count - 1;
}

void DestroyMeshObject(GameWorld& world, uint32_t object)
{
	world.meshes[object] = world.meshes[world.meshes.count - 1];
	world.meshes.count--;
}
