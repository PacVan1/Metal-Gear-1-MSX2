#include "precomp.h"
#include "Enemy.h"

#include "Game.h" 

Enemy::Enemy(AnimationSpriteSheet& spriteSheet, int const dirAnimCount) :
	Actor(spriteSheet, dirAnimCount), 
	mOptimalFacing(EAST),
	mOptimalFacingOpp(false) 
{}

bool Enemy::TargetInLine() const
{
	int2 dir = cardinalToInt2(mFacing); 
	int2 targetCoord = PixelToTile(Game::sPlayer.GetPositionInt()); 
	int2 soldierCoord = PixelToTile(GetPositionInt());  
	if (int2(targetCoord.x * dir.y, targetCoord.y * dir.x) != int2(soldierCoord.x * dir.y, soldierCoord.y * dir.x))
	{
		return false;
	}

	// account for the longest span possible: 
	for (int i = 0; i < Tilemap::COLUMNS; i++)
	{
		int2 coord = soldierCoord + dir * i;
		uint8_t tileState = Game::GetTilemap().GetTileState(coord); 
		if (tileState == TilePalette::Tile::SOLID) return false;
		if (coord == targetCoord) return true;
	}

	return false; 
}

int Enemy::DecideFacing()
{
	float2 const dir	= Game::sPlayer.GetPosition() - GetPosition();
	float2 const absDir = fabs(dir);

	int2 const longestComp	= (absDir.x > absDir.y) ? 
		int2(static_cast<int>(dir.x / absDir.x), 0) :
		int2(0, static_cast<int>(dir.y / absDir.y));

	int2 const shortestComp = (absDir.x < absDir.y) ? 
		int2(static_cast<int>(dir.x / absDir.x), 0) :
		int2(0, static_cast<int>(dir.y / absDir.y));

	mOptimalFacing				= int2ToCardinal( longestComp); 
	int const shortestFacing	= int2ToCardinal( shortestComp);
	int const longOppFacing		= int2ToCardinal(-longestComp);
	int const shortOppFacing	= int2ToCardinal(-shortestComp);

	if (!mBboxTile.DetectTilemap(mOptimalFacing, mSpeed))
	{
		mOptimalFacingOpp = false;
		return mOptimalFacing;
	}
	else if (!mBboxTile.DetectTilemap(mFacing, mSpeed)) 
	{
		mOptimalFacingOpp = false;
	}
	else if (!mBboxTile.DetectTilemap(shortestFacing, mSpeed)) 
	{
		mOptimalFacingOpp = false;
		return shortestFacing; 
	}
	else if (!mBboxTile.DetectTilemap(shortOppFacing, mSpeed))
	{
		mOptimalFacingOpp = true;
		return shortOppFacing;
	}
	else if (!mBboxTile.DetectTilemap(longOppFacing, mSpeed))
	{
		mOptimalFacingOpp = true;
		return longOppFacing; 
	}
	return mFacing; 
}

void Enemy::Pursue(float const dt)
{
	// is the guard not facing the most optimal direction
	if (!mOptimalFacingOpp && mFacing != mOptimalFacing)
	{
		// try to go in the most optimal direction
		if (!mBboxTile.DetectTilemap(mOptimalFacing, mSpeed * dt)) 
		{
			SetFacing(mOptimalFacing);  
		}
	}

	// is the guard hitting a wall OR reaching the target
	if (mBboxTile.DetectTilemap(mFacing, mSpeed * dt) || 
		cardinalToInt2(mFacing) * GetPositionInt() == cardinalToInt2(mFacing) * Game::sPlayer.GetPositionInt())
	{
		SetFacing(DecideFacing());  
	}

	Move(dt);
}
