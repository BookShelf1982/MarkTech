#pragma once

#include <math3D.h>
#include <stdint.h>
#include "data_structures.h"
#include "renderer.h"

#define MAX_OBJECTS 64

typedef enum {
	BUTTONS_ESC,
	BUTTONS_FORWARD,
	BUTTONS_BACKWARD,
	BUTTONS_LEFT,
	BUTTONS_RIGHT,
	BUTTONS_LOOK_LEFT,
	BUTTONS_LOOK_RIGHT
} Buttons;

typedef struct {
	uint8_t buttons;
} InputState;

typedef struct {
	uint32_t meshHandle;
	vec3f_t origin;
} MeshObject;

typedef struct {
	InputState input;
	struct {
		vec3f_t playerOrigin;
		vec3f_t playerRotation;
	} player;
	FixedArray<MeshObject, MAX_OBJECTS> meshes;
	SceneSnapshot snapshot;
} GameWorld;

void TickGameWorld(GameWorld& world, float dt);
uint32_t CreateMeshObject(GameWorld& world);
void DestroyMeshObject(GameWorld& world, uint32_t object);