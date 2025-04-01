#pragma once

#include "AABB.h" 

class GameObject
{
public:
	AABB bbox; 

public:
	GameObject(int2 size, int2 offset);
	virtual void	SetPosition(float2 const position);
	float2			GetPosition() const { return bbox.GetCenter(); }
	int2			GetPositionInt() const { return bbox.GetCenterInt(); }
};

