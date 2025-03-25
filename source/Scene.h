#pragma once

#include "TileMap.h"
#include "Sequence.h"

struct Scene
{
	Tilemap const*	tilemap;
	Sequence**		paths;
	uint			soldierCount = 0;

	Scene() = default;
	Scene(char const* path);
};

