#pragma once

#include "Actor.h" 
#include "Player.h" 

class Enemy : public Actor
{
public:
	inline static Player const* target = nullptr;

public:
	int		optimalFacing;
	bool	oppositeFacing;

public:
	void	DecideCardinal();
	bool	TargetInLine() const;

protected: 
			Enemy(AnimationSpriteSheet& sheet, uint dirAnimCount);
	void	Pursue(float const dt);
};

