#pragma once

class BoundingBox
{
public:
	int2 size;
	int2 offset; 
	int2 topLeft; 
	int2 bottomRight; 

public:
	BoundingBox(int2 size, int2 offset);  
	void Update(int2 position);  
	void Render(Surface* screen); 
	bool AABB(BoundingBox const& other) const; 
	bool AABB(float2 point) const; 
};

class AABB
{
public:
	static bool Check(BoundingBox const& bbox0, BoundingBox const& bbox1);    
	static bool Check(BoundingBox const& bbox0, f32 right, f32 top, f32 left, f32 bottom);
	static bool Check(float2 point, f32 right, f32 top, f32 left, f32 bottom); 
};

