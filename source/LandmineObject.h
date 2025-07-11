#pragma once

#include "SpriteSheet.h" 
#include "AnimatedSprite.h" 
#include "AABB.h"
#include "Actor.h" 

enum landmineStates
{
	LANDMINE_STATES_IDLE,
	LANDMINE_STATES_EXPLODING,
	LANDMINE_STATES_EXPLODED
};

class LandmineObject
{
public:
	inline static auto	sSpriteSheetExplosion	= AnimationSpriteSheet("assets/spritesheets/explosion/explosion.animsheet");
	inline static auto	sColorPaletteExplosion	= ColorPalette8("assets/color_palettes/explosion.cpalette");  

public:
	AnimatedSprite	mExplosionDisplay; 
	AABB			mBboxDetonate;
	AABB			mBboxExplosion;
	Alarm			mCycleAlarm = Alarm(0.1f); 
	float2			mPosF;
	int2			mPosI;

private: 
	int				mState; 

public:
					LandmineObject();
	void			Update(float const dt); 
	void			Render(Surface8* screen) const; 
	void			Detonate();
	bool			Hit(Actor& actor);
	void			Reset(); 

	void			SetPosition(float2 const position);

	inline int		GetState() const	{ return mState; }
	inline bool		IsExploded() const	{ return mState == LANDMINE_STATES_EXPLODED; }
};

