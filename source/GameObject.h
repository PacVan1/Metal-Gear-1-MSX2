#pragma once

#include "AABB.h" 

class GameObject
{
public:
	AABB bbox; 

public:
	GameObject(int2 size, int2 offset);
	void SetPosition(float2 const position);
	inline float2	GetPosition() const { return bbox.fPos; }
	inline int2		GetPositionInt() const { return bbox.iPos; }
};

