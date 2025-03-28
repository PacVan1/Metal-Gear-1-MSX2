#pragma once

#include "GameObject.h" 
#include "TileMap.h"

class Scene; 
struct PassageSharedData; 

class Passage : public GameObject
{
public:
	Passage*			other; 
	PassageSharedData*	sharedData;
	int2				spawnPosition;
	Scene*				scene; 

public:
	Passage();
	void TryEnter() const;  
};

struct PassageSharedData
{
	bool		unlocked;
};

