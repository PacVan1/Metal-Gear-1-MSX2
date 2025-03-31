#include "precomp.h"
#include "Enemy.h"

#include "TileMap.h" 

Enemy::Enemy(AnimationSpriteSheet& sheet, uint dirAnimCount) :
	Actor(sheet, dirAnimCount),
	optimalFacing(EAST) 
{}

bool Enemy::TargetInLine() const
{
	int2 dir = CardinalToInt2(facing);
	int2 targetCoord = PixelToTile(target->bboxTile.iPos);
	int2 soldierCoord = PixelToTile(bboxTile.iPos);
	if (int2(targetCoord.x * dir.y, targetCoord.y * dir.x) != int2(soldierCoord.x * dir.y, soldierCoord.y * dir.x))
	{
		return false;
	}

	// account for the longest span possible: 
	for (int i = 0; i < Tilemap::COLUMNS; i++)
	{
		int2 coord = soldierCoord + dir * i;
		uint8_t tileState = AABB::currentTilemap->GetTileState(coord);
		if (tileState == TilePalette::Tile::NONE) break;
		if (tileState == TilePalette::Tile::SOLID) return false;
		if (coord == targetCoord) return true;
	}

	return true;
}

void Enemy::DecideCardinal()
{
	float2 dir = target->GetPosition() - GetPosition();
	float2 absDir = fabs(dir);

	int2 longestComp = (absDir.x > absDir.y) ? int2(dir.x / absDir.x, 0) : int2(0, dir.y / absDir.y);
	int2 shortestComp = (absDir.x < absDir.y) ? int2(dir.x / absDir.x, 0) : int2(0, dir.y / absDir.y);

	optimalFacing		= Int2ToCardinal(longestComp);
	int shortestFacing	= Int2ToCardinal(shortestComp);
	int longOppFacing	= Int2ToCardinal(-longestComp);
	int shortOppFacing	= Int2ToCardinal(-shortestComp);

	if (!bboxTile.DetectTilemap(optimalFacing, speed))
	{
		oppositeFacing = false;
		facing = optimalFacing;
	}
	else if (!bboxTile.DetectTilemap(facing, speed))
	{
		oppositeFacing = false;
	}
	else if (!bboxTile.DetectTilemap(shortestFacing, speed))
	{
		oppositeFacing = false;
		facing = shortestFacing;
	}
	else if (!bboxTile.DetectTilemap(shortOppFacing, speed))
	{
		oppositeFacing = true;
		facing = shortOppFacing;
	}
	else if (!bboxTile.DetectTilemap(longOppFacing, speed))
	{
		oppositeFacing = true;
		facing = longOppFacing;
	}
}

void Enemy::Pursue(float const dt)
{
	// is the guard not facing the most optimal direction
	if (!oppositeFacing && facing != optimalFacing)
	{
		// try to go in the most optimal direction
		if (!bboxTile.DetectTilemap(optimalFacing, speed * dt))
		{
			facing = optimalFacing;
			SetAnimation();
		}
	}

	// is the guard hitting a wall OR reaching the target
	if (bboxTile.DetectTilemap(facing, speed * dt) ||
		CardinalToInt2(facing) * GetPositionInt() == CardinalToInt2(facing) * target->GetPositionInt())
	{
		DecideCardinal();
		SetAnimation();
	}

	Move(dt);
}
