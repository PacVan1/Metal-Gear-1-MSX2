#pragma once

#include "Animator.h" 
#include "SpriteSheet.h" 

class AnimatedSprite
{
public:
	AnimationSpriteSheet&	sheet;
	Animator				animator;
	uint					frame; 

public:
	AnimatedSprite(AnimationSpriteSheet& sheet);   
	void Render(Surface8* screen, int x, int y) const;  
};

