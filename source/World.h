#pragma once

#include "Cardinals.h"
#include "Scene.h"
#include "Passage.h" 

class World
{
public:
	Scene*				mScenes;	
	PassageSharedData*	mSharedData;
	int					mPassageCount;
	int					mSceneCount;
	int					mAdjSceneCount; 
	int					mColumns;
	int					mRows;

public: 
				World();
	inline int2 GetSceneCoordinate(int const idx) const		{ return { idx % mColumns, idx / mColumns }; }
	inline int	GetSceneIdx(int2 const coordinate) const	{ return coordinate.x + coordinate.y * mColumns; }
	inline int	GetSceneCount() const						{ return mSceneCount; }
	inline int	GetAdjSceneCount() const					{ return mAdjSceneCount; }
};

