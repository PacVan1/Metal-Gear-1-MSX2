#pragma once

#include "Soldier.h" 

enum soldierTypes : uint8_t
{
	SOLDIER_TYPES_GRAY,
	SOLDIER_TYPES_RED,
	SOLDIER_TYPES_COUNT
};

struct SoldierTypes
{
	AnimationSpriteSheet	mSpriteSheet = AnimationSpriteSheet("assets/spritesheets/soldier/soldier.animsheet");
	int						mBehaviorToAnimState[SOLDIER_BEHAVIOR_COUNT] = { 0, 1, 1, 0, 0, 0, 0 };
	SoldierTypeData			mTypeData[SOLDIER_TYPES_COUNT];

	SoldierTypes();
};


