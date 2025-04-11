#include "precomp.h"
#include "AABB.h"

#include "Game.h"

bool AABB::Detect(AABB const& bbox1, AABB const& bbox2)  
{
	return	bbox1.GetTopLeft().x <= bbox2.GetBottomRight().x &&
			bbox1.GetBottomRight().x >= bbox2.GetTopLeft().x &&
			bbox1.GetTopLeft().y <= bbox2.GetBottomRight().y &&
			bbox1.GetBottomRight().y >= bbox2.GetTopLeft().y;
}

bool AABB::DetectGroup(AABB const& bbox1, AABB const& bbox2)
{
	return (bbox1.mGroup == bbox2.mGroup && Detect(bbox1, bbox2));
}

bool AABB::Detect(AABB const& bbox, float const right, float const top, float const left, float const bottom)
{
	return	bbox.GetTopLeft().x <= right	&& 
			bbox.GetBottomRight().x >= left &&
			bbox.GetTopLeft().y <= bottom	&&
			bbox.GetBottomRight().y >= top;
}

bool AABB::Detect(float2 const point, float const right, float const top, float const left, float const bottom) 
{
	return	point.x >= left    && 
			point.y >= top	   && 
			point.x <= right   &&
			point.y <= bottom;   
}

bool AABB::Detect(int2 const point, int const right, int const top, int const left, int const bottom)
{
	return	point.x >= left    && 
			point.y >= top	   && 
			point.x <= right   && 
			point.y <= bottom;
}

Area AABB::Overlap(AABB const& bbox1, AABB const& bbox2)
{
	Area area; 
	area.mLeft		= max(bbox1.GetTopLeft().x, bbox2.GetTopLeft().x); 
	area.mTop		= max(bbox1.GetTopLeft().y, bbox2.GetTopLeft().y); 
	area.mRight		= min(bbox1.GetBottomRight().x, bbox2.GetBottomRight().x);
	area.mBottom	= min(bbox1.GetBottomRight().y, bbox2.GetBottomRight().y);
	return area; 
}

AABB::AABB(float2 const* position) :
	mGroup(COLLISION_GROUPS_NONE),
	mPos(position),
	mHalfSize(0.0f),
	mTopLeft(0.0f),
	mBottomRight(0.0f),
	mOffset(0),
	mSize(0)
{
	Update(); 
}

AABB::AABB(float2 const* position, int2 const size, float2 const offset) :
	mGroup(COLLISION_GROUPS_NONE),
	mPos(position),
	mHalfSize(static_cast<float2>(size) / 2.0f),  
	mTopLeft(0.0f), 
	mBottomRight(0.0f),
	mOffset(offset),
	mSize(size)
{
	Update();
}

void AABB::Update()
{
	mTopLeft = *mPos + mOffset; 
	mBottomRight = mTopLeft + mSize - int2(1); 
}

void AABB::Render(Surface8* screen, uint8_t const cMiss, uint8_t const cHit) const
{
#if DEBUG_MODE
	int2 const topLeft = static_cast<int2>(mTopLeft);
	int2 const bottomRight = static_cast<int2>(mBottomRight);
	int2 const pos = static_cast<int2>(*mPos);
	if (mHit)
	{
		screen->Box(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, cHit);
		screen->Plot(pos.x, pos.y, cHit);
}
	else
	{
		screen->Box(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, cMiss);
		screen->Plot(pos.x, pos.y, cMiss); 
	}
#endif
}

void AABB::Resize(int2 const size, int2 const offset)
{
	mSize		= size;
	mOffset		= static_cast<float2>(offset); 
	mHalfSize	= static_cast<float2>(size) / 2.0f;
	Update(); 
}

#if DEBUG_MODE 
bool AABB::Detect(AABB& other)
{
	//mHit = false;
	//other.mHit = false; // TODO TEMP
	bool const hit =	GetTopLeft().x <= (other.GetBottomRight().x + 1.0f) &&
						(GetBottomRight().x + 1.0f) >= other.GetTopLeft().x &&
						GetTopLeft().y <= (other.GetBottomRight().y + 1.0f) &&
						(GetBottomRight().y + 1.0f) >= other.GetTopLeft().y;
	this->mHit = (hit) ? hit : this->mHit;
	other.mHit = (hit) ? hit : other.mHit;
	return hit;
}
#else 
bool AABB::Detect(AABB const& other) const
{
	return	mTopLeft.x <= other.mBottomRight.x &&  
			mBottomRight.x >= other.mTopLeft.x &&  
			mTopLeft.y >= other.mBottomRight.y &&
			mBottomRight.y <= other.mTopLeft.y; 
} 
#endif

bool AABB::Detect(float2 const point) const
{
	return	point.x >= GetTopLeft().x     &&
			point.y >= GetTopLeft().y	  &&
			point.x <= GetBottomRight().x &&
			point.y <= GetBottomRight().y;
}

bool AABB::DetectTilemap() const
{
	TileArea area = SubPixelToTileArea(*mPos - mHalfSize, *mPos + mHalfSize);
	return DetectTilemap(area);  
}

bool AABB::DetectTilemap(int const cardinal, float const distance) const
{
	int2 const dir = cardinalToInt2(cardinal);
	float2 const dirf = cardinalToFloat2(cardinal);
	float2 const pos = *mPos + float2(-1.0f, 1.0f); 

	// going horizontal
	if (dir.y == 0)
	{
		float2 const edge = pos + dirf * mHalfSize.x;
		float2 const nextEdge = edge + distance * dirf;
		TileArea area = SubPixelToTileArea(float2(edge.x, pos.y - mHalfSize.y + 1),
			float2(nextEdge.x, pos.y + mHalfSize.y));

		int xx = (dir.x < 0) ? area.brCoord.x : area.tlCoord.x;
		int distX = area.brCoord.x - area.tlCoord.x + 1;
		for (int i = 0; i < distX; xx += dir.x, i++)
		{
			for (int yy = area.tlCoord.y; yy <= area.brCoord.y; yy++)
			{
				uint8_t tileState = Game::GetTilemap().GetTileState({ xx, yy });  
				if (tileState == TilePalette::Tile::SOLID)
				{
					return true;
				}
			}
		}
	}
	// going vertical
	else
	{
		float2 edge = pos + dirf * mHalfSize.y;
		float2 nextEdge = edge + distance * dirf;
		TileArea area = SubPixelToTileArea(float2(edge.x - mHalfSize.x + 1, edge.y),
			float2(nextEdge.x + mHalfSize.x, nextEdge.y));

		int yy = (dir.y < 0) ? area.brCoord.y : area.tlCoord.y;
		int distY = area.brCoord.y - area.tlCoord.y + 1;
		for (int i = 0; i < distY; yy += dir.y, i++)
		{
			for (int xx = area.tlCoord.x; xx <= area.brCoord.x; xx++)
			{
				uint8_t tileState = Game::GetTilemap().GetTileState({ xx, yy });
				if (tileState == TilePalette::Tile::SOLID) 
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool AABB::DetectTilemap(TileArea const& area)
{
	for (int y = area.tlCoord.y; y <= area.brCoord.y; y++) for (int x = area.tlCoord.x; x <= area.brCoord.x; x++)
	{
		uint8_t tileState = Game::GetTilemap().GetTileState({ x, y });
		if (tileState == TilePalette::Tile::SOLID) return true;
	}
	return false; 
}
