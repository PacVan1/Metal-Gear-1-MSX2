#pragma once

#include "AABB.h"
#include "TileMap.h"

class Scene; 
struct PassageSharedData; 

struct PassageProps
{
	MetaTile	dataUnlocked;
	MetaTile	dataLocked;
	int2		spawnPosition; 
};

class Passage
{
public:
	AABB				mBbox;
	float2				mPosF;
	int2				mPosI;
	int2				mTileCoord; 
	Passage*			other; 
	PassageSharedData*	sharedData;
	PassageProps const&	props; 
	int2				spawnPosition;
	//Scene*				scene;
	int					mSceneIdx;  

public:
	static void Connect(PassageSharedData* sharedData, Passage* passage1, Passage* passage2); 

public:
	Passage(PassageProps const& props);
	void Render(Surface8* screen) const; 
	void TryEnter() const;
	void SetPosition(int2 const position); 
};

struct PassageSharedData
{
	bool		mUnlocked;
	int			mAccessLevel;
};