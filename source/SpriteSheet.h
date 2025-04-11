#pragma once

#include "Animation.h"

class SpriteSheet : public Sprite
{
public:
	int mFrameCount; 
	int mFrameWidth; 

public: 
			SpriteSheet() = default;
			SpriteSheet(char const* path);  
	void	RenderFrame(Surface8* screen, int x, int y, uint frame) const;  
};

class AnimationSpriteSheet : public SpriteSheet 
{
public:
	AnimationSet mAnimSet; 

public:
	AnimationSpriteSheet() = default; 
	AnimationSpriteSheet(char const* path);
};