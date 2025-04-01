#pragma once

#include "Animator.h" 
#include "SpriteSheet.h" 
#include "AABB.h"

class AnimatedSprite
{
public:
	AnimationSpriteSheet&	sheet;
	Animator				animator;
	uint					frame; 

public:
	AnimatedSprite(AnimationSpriteSheet& sheet);   
	void Render(Surface8* screen, int x, int y) const;
	uint8_t GetPixel(int2 coord) const
	{
		if (AABB::Detect(coord, sheet.frameWidth - 1, 0, 0, sheet.sprite.height - 1))
		{
			return sheet.sprite.palette->indices[sheet.sprite.pixels[frame * sheet.frameWidth + coord.x + coord.y * sheet.sprite.width]]; 
		}
	}
};

