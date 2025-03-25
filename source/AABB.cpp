#include "precomp.h"
#include "AABB.h"

#include "Settings.h"
#include "TileMap.h" 

bool AABB::Detect(AABB const& bbox0, AABB const& bbox1)  
{
	return	bbox0.topLeft.x <= bbox1.bottomRight.x &&    
			bbox0.bottomRight.x >= bbox1.topLeft.x && 
			bbox0.topLeft.y <= bbox1.bottomRight.y &&
			bbox0.bottomRight.y >= bbox1.topLeft.y;
}

bool AABB::DetectGroup(AABB const& bbox0, AABB const& bbox1)
{
	return (bbox0.group == bbox1.group && Detect(bbox0, bbox1));
}

bool AABB::Detect(AABB const& bbox0, f32 right, f32 top, f32 left, f32 bottom)
{
	return	bbox0.topLeft.x <= right &&
			bbox0.bottomRight.x >= left &&  
			bbox0.topLeft.y <= bottom && 
			bbox0.bottomRight.y >= top;  
}

bool AABB::Detect(float2 point, f32 right, f32 top, f32 left, f32 bottom)
{
	return	point.x >= left && 
			point.y >= top && 
			point.x <= right &&
			point.y <= bottom;   
}

/// checks if a certain point is inside included bounds
bool AABB::Detect(int2 point, int right, int top, int left, int bottom)
{
	return	point.x >= left && 
		point.y >= top && 
		point.x <= right && 
		point.y <= bottom;
}

AABB::AABB(int2 size, int2 offset) :
	SIZE(size),  
	HALF_SIZE(size / 2.0f), 
	OFFSET(offset),  
	topLeft(0, 0), 
	bottomRight(0, 0),
	group(NONE)
{}

bool AABB::Move(int cardinal, f32 distance) 
{
	// for now any object wanting to move using AABB gets blocked by tiles:
	if (currentTilemap)
	{
		if (!DetectTilemap(cardinal, distance))
		{
			fPos = fPos + CardinalToFloat2(cardinal) * distance;
			iPos = (int2)fPos;
			topLeft = int2(fPos + OFFSET);
			bottomRight = topLeft + SIZE;
			return false; 
		}
	}
	return true; 
}

void AABB::Update(float2 const position)
{
	fPos = position;
	iPos = (int2)fPos; 
	topLeft = int2(position + OFFSET);
 	bottomRight = topLeft + SIZE;
}

void AABB::Render(Surface8* screen) const
{
#if DEBUG_MODE     
	int2 pos = topLeft - OFFSET;    
	if (hit)
	{
		screen->Box(topLeft.x + 1, topLeft.y - 1, bottomRight.x, bottomRight.y - 2, hitColor); 
		screen->Plot(pos.x, pos.y, hitColor);   
	}
	else
	{
		screen->Box(topLeft.x + 1, topLeft.y - 1 , bottomRight.x, bottomRight.y - 2, missColor);
		screen->Plot(pos.x, pos.y, missColor);   
	}
#endif
}

#if DEBUG_MODE 
bool AABB::Detect(AABB& other)
{
	hit = false; 
	bool hit = topLeft.x <= other.bottomRight.x &&
			   bottomRight.x >= other.topLeft.x && 
			   topLeft.y <= other.bottomRight.y &&
			   bottomRight.y >= other.topLeft.y;
	this->hit = (hit) ? hit : this->hit; 
	other.hit = (hit) ? hit : other.hit; 
	return hit;
}
#else 
bool AABB::Detect(AABB const& other) const
{
	return	topLeft.x <= other.bottomRight.x &&  
			bottomRight.x >= other.topLeft.x &&  
			topLeft.y >= other.bottomRight.y &&
			bottomRight.y <= other.topLeft.y; 
} 
#endif

bool AABB::Detect(float2 point) const
{
	return	point.x >= topLeft.x && 
			point.y >= topLeft.y && 
			point.x <= bottomRight.x && 
			point.y <= bottomRight.y;  
} 

bool AABB::DetectTilemap(int card, f32 dist) const
{
	int2 const dir = CardinalToInt2(card);
	float2 const dirf = CardinalToFloat2(card);

	// going horizontal
	if (dir.y == 0)
	{
		float2 edge = GetCenter() + dirf * HALF_SIZE.x;
		float2 nextEdge = edge + dist * dirf;
		TileArea area = SubPixelToTileArea(float2(edge.x, GetCenter().y - HALF_SIZE.y + 1),
			float2(nextEdge.x, GetCenter().y + HALF_SIZE.y));

		int xx = (dir.x < 0) ? area.brCoord.x : area.tlCoord.x;
		int distX = area.brCoord.x - area.tlCoord.x + 1;
		for (int i = 0; i < distX; xx += dir.x, i++)
		{
			for (int yy = area.tlCoord.y; yy <= area.brCoord.y; yy++)
			{
				uint8_t tileState = currentTilemap->GetTileState({ xx, yy }); 
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
		float2 edge = GetCenter() + dirf * HALF_SIZE.y;
		float2 nextEdge = edge + dist * dirf;
		TileArea area = SubPixelToTileArea(float2(edge.x - HALF_SIZE.x + 1, edge.y),
			float2(nextEdge.x + HALF_SIZE.x, nextEdge.y));

		int yy = (dir.y < 0) ? area.brCoord.y : area.tlCoord.y;
		int distY = area.brCoord.y - area.tlCoord.y + 1;
		for (int i = 0; i < distY; yy += dir.y, i++)
		{
			for (int xx = area.tlCoord.x; xx <= area.brCoord.x; xx++)
			{
				uint8_t tileState = currentTilemap->GetTileState({ xx, yy }); 
				if (tileState == TilePalette::Tile::SOLID) 
				{
					return true;
				}
			}
		}
	}

	return false;
}

inline float2 AABB::GetCenter() const
{
	return fPos;   
}

inline float2 AABB::GetTopLeft() const
{
	return (float2)topLeft;   
}

inline float2 AABB::GetBottomRight() const
{
	return (float2)bottomRight;
}
