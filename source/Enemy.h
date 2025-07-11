#pragma once

#include "Actor.h" 
#include "Player.h" 

class Enemy : public Actor
{
protected:   
	int		mOptimalFacing;
	bool	mOptimalFacingOpp; 

public:
	int		DecideFacing(); 
	bool	TargetInLine() const;

protected: 
			Enemy(AnimationSpriteSheet& spriteSheet, int const dirAnimCount);
	void	Pursue(float const dt);
};

