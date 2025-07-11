#include "precomp.h"
#include "LandmineObject.h"

#include "Game.h"

LandmineObject::LandmineObject() :
	mExplosionDisplay(&sSpriteSheetExplosion),
	mBboxDetonate(&mPosF, { 16, 16 }, { -8, -8 }),
	mBboxExplosion(&mPosF, { sSpriteSheetExplosion.mFrameWidth, sSpriteSheetExplosion.height }, { -sSpriteSheetExplosion.mFrameWidth / 2.0f, -sSpriteSheetExplosion.height / 2.0f}),
	mPosF(0.0f),
	mPosI(0),
	mState(LANDMINE_STATES_IDLE)
{
	mExplosionDisplay.mAnimator.mRate = 100;
	mBboxDetonate.mGroup	= COLLISION_GROUPS_ENEMY;  
	mBboxExplosion.mGroup	= COLLISION_GROUPS_ENEMY;
}

void LandmineObject::Update(float const dt)
{
	switch (mState)
	{
	case LANDMINE_STATES_EXPLODING:
	{
		mExplosionDisplay.mAnimator.Play(dt);
		//if (mCycleAlarm.Elapsed()) { sColorPaletteExplosion.Cycle(); mCycleAlarm.Reset(); }
		if (mExplosionDisplay.mAnimator.IsFinished())
		{
			mState = LANDMINE_STATES_EXPLODED;
		}
		break;
	}
	default: break; 
	}
}

void LandmineObject::Render(Surface8* screen) const
{
	switch (mState)
	{
	case LANDMINE_STATES_IDLE:
	{
		Game::sInventory.mItems[ITEM_TYPES_LAND_MINE]->Render(screen, mPosI.x + mBboxDetonate.GetOffset().x - 8, mPosI.y + mBboxDetonate.GetOffset().y);
		mBboxDetonate.Render(screen); 
		break;
	}
	case LANDMINE_STATES_EXPLODING:
	{
		int2 const pos = static_cast<int2>(mBboxExplosion.GetTopLeft());  
		mExplosionDisplay.Render(screen, pos.x, pos.y);
		mBboxExplosion.Render(screen);
		break;
	}
	default: break; 
	}
}

void LandmineObject::Detonate() 
{
	mState = LANDMINE_STATES_EXPLODING;
	mExplosionDisplay.mAnimator.Reset();
}

bool LandmineObject::Hit(Actor& actor)
{
	if (mState == LANDMINE_STATES_IDLE)
	{
		if (AABB::DetectGroup(mBboxDetonate, actor.GetBboxTile()))
		{
			Detonate();
		}
	}
	else if (mState == LANDMINE_STATES_EXPLODING)
	{
		if (AABB::DetectGroup(mBboxExplosion, actor.GetBboxTile()))
		{
			return true; 
		}
	}
	return false;
}

void LandmineObject::Reset()
{
	mState = LANDMINE_STATES_IDLE;
	mExplosionDisplay.mAnimator.Reset(); 
}

void LandmineObject::SetPosition(float2 const position)
{
	mPosF = position;
	mPosI = static_cast<int2>(mPosF);
	mBboxDetonate.Update();
	mBboxExplosion.Update();
}
