#pragma once

#include "Cardinals.h"

class TileMap; 
class Tilemap;

class AABB 
{
public:
	enum group
	{
		NONE, PLAYER, ENEMY, COUNT
	};

public:
	inline static Tilemap const* currentTilemap = nullptr;   
#if DEBUG_MODE
	static uint8_t const hitColor = 180;
	static uint8_t const missColor = 140;
#endif

public:
	int2 const		SIZE;
	float2 const	HALF_SIZE;       
	int2 const		OFFSET;  

public:
	float2	fPos;
	int2	iPos;
	int		group; 

private:
	int2	topLeft; 
	int2	bottomRight; 
#if DEBUG_MODE
	bool	hit = false; 
#endif

public:
	static bool Detect(AABB const& bbox0, AABB const& bbox1);
	static bool DetectGroup(AABB const& bbox0, AABB const& bbox1); 
	static bool Detect(AABB const& bbox0, f32 right, f32 top, f32 left, f32 bottom);
	static bool Detect(float2 point, f32 right, f32 top, f32 left, f32 bottom);
	static bool Detect(int2 point, int right, int top, int left, int bottom); 

public:
	AABB() = default; 
	AABB(int2 size, int2 offset);
	bool Move(int cardinal, f32 distance);
	void Update(float2 const position); 
	void Render(Surface8* screen) const;  
#if DEBUG_MODE 
	bool Detect(AABB& other); 
#else
	bool Detect(AABB const& other) const; 
#endif
	bool Detect(float2 point) const;   
	bool DetectTilemap(int cardinal, f32 distance) const; 
	inline float2 GetCenter() const; 
	inline float2 GetTopLeft() const; 
	inline float2 GetBottomRight() const; 
};

