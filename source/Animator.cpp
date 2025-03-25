#include "precomp.h"
#include "Animator.h"

#include "AnimatedSprite.h" 

Animator::Animator(AnimatedSprite& sprite) :
	sprite(sprite)
{
	SetAnimation(0);
} 

void Animator::Play(float const dt)
{
	if (!anim->hasDuration) 
	{
		return;
	}

	finished = false;
	time += dt; 
	if (static_cast<uint>(time) >= rate)   
	{
		time -= (float)rate;  
		sprite.frame++; 
		if (sprite.frame > anim->finalFrame) 
		{
			sprite.frame = anim->startFrame; 
			finished = true;
		}
	}
}

void Animator::SetAnimation(uint const idx)
{
	animIdx = idx;   
	anim = &sprite.sheet.animSet.anims[idx]; 
	Reset();
}

void Animator::Reset()
{
	finished = false;
	time = 0.0f; 
	sprite.frame = anim->startFrame; 
}