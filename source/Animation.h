#pragma once

// a struct for storing simple animation data
struct Animation
{
	uint	startFrame;
	uint	finalFrame; 
	bool	hasDuration;

	Animation() = default; 
	Animation(uint frameCount, uint startFrame, bool hasDuration);  
};

// a struct for bundling different animations together 
struct AnimationSet
{
	uint		count; 
	Animation*	anims; 

	AnimationSet() = default; 
	AnimationSet(char const* path); 
};
