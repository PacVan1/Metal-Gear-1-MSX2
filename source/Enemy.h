#pragma once

#include "Actor.h" 
#include "Player.h" 

class Enemy : public Actor
{
public:
	inline static Player const* target = nullptr;

public:
	int optimalFacing;
	int oppositeFacing;

public:
	void	DecideCardinal();
	bool	TargetInLine() const;

protected: 
			Enemy(AnimationSpriteSheet& sheet, uint dirAnimCount);
	void	Persue(float const dt);  
};

