#pragma once

#include "Animation.h"

class SpriteSheet
{
public:
	int frameCount; 
	int frameWidth; 
	Sprite sprite; 

public: 
	SpriteSheet() = default; 
	SpriteSheet(char const* path);  
	void Render(Surface8* screen, int x, int y, uint frame) const;  
};

class AnimationSpriteSheet : public SpriteSheet 
{
public:
	AnimationSet animSet; 

public:
	AnimationSpriteSheet(char const* path);
};