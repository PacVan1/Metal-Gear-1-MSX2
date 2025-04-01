#pragma once

#include "TileMap.h"
#include "Sequence.h"
#include "Item.h"

struct Scene
{
	Tilemap*		tilemap;
	Sequence**		paths;
	ItemObject**	items;
	uint			itemCount = 0; 
	uint			soldierCount = 0;

	Scene() = default;
	Scene(char const* path);
};

