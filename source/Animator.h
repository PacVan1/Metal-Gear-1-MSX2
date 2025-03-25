#pragma once

#include "Animation.h" 

class AnimatedSprite;

class Animator
{
public:
	uint				rate = 200;
	AnimatedSprite&		sprite; 
	Animation const*	anim; // for readability and efficiency
	uint				animIdx; 
	float				time;
	bool				finished;  

public:
				Animator(AnimatedSprite& sprite); 
	void		Play(float const dt);
	void		SetAnimation(uint const idx); 
	void		Reset(); 
	inline bool IsFinished() const { return finished; }
};

