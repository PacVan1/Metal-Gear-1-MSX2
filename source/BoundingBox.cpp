#include "precomp.h"
#include "BoundingBox.h"

#include "Settings.h"

BoundingBox::BoundingBox(int2 size, int2 offset) : 
	size(size), 
	offset(size),
	topLeft(0, 0), 
	bottomRight(0, 0) 
{}

void BoundingBox::Update(int2 position) 
{
	topLeft		= position + offset;
	bottomRight = topLeft + size;
}

void BoundingBox::Render(Surface* screen)
{
#if DEBUG_MODE     
	screen->Box(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 0xff0000);   
#endif
}

bool BoundingBox::AABB(BoundingBox const& other) const
{
	return	topLeft.x <= other.bottomRight.x &&  
			bottomRight.x >= other.topLeft.x &&  
			topLeft.y >= other.bottomRight.y &&
			bottomRight.y <= other.topLeft.y; 
}

bool BoundingBox::AABB(float2 point) const 
{
	return	point.x >= topLeft.x && 
			point.y >= topLeft.y && 
			point.x <= bottomRight.x && 
			point.y <= bottomRight.y;  
}

bool AABB::Check(BoundingBox const& bbox0, BoundingBox const& bbox1) 
{
	return	bbox0.topLeft.x <= bbox1.bottomRight.x && 
			bbox0.bottomRight.x >= bbox1.topLeft.x &&
			bbox0.topLeft.y >= bbox1.bottomRight.y &&
			bbox0.bottomRight.y <= bbox1.topLeft.y;
}

bool AABB::Check(BoundingBox const& bbox0, f32 right, f32 top, f32 left, f32 bottom)  
{
	return	bbox0.topLeft.x <= right &&
			bbox0.bottomRight.x >= left &&  
			bbox0.topLeft.y >= bottom && 
			bbox0.bottomRight.y <= top;  
}

bool AABB::Check(float2 point, f32 right, f32 top, f32 left, f32 bottom)
{
	return	point.x >= left && 
			point.y >= top && 
			point.x <= right &&
			point.y <= bottom;   
}
