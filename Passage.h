#pragma once

#include "GameObject.h" 
#include "TileMap.h"

class Scene; 
struct PassageSharedData; 

struct PassageProps
{
	MetaTile	dataUnlocked;
	MetaTile	dataLocked;
	int2		spawnPosition; 
};

class Passage : public GameObject
{
public:
	Passage*			other; 
	PassageSharedData*	sharedData;
	PassageProps const&	props; 
	int2				spawnPosition;
	Scene*				scene; 

public:
	Passage(PassageProps const& props);
	void SetPosition(float2 const position) override;
	void Render(Surface8* screen) const; 
	void TryEnter() const;  
};

struct PassageSharedData
{
	bool		unlocked;
	int			accessLevel; 
};