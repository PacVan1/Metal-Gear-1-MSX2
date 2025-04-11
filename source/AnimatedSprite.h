#pragma once

#include "Animator.h" 
#include "SpriteSheet.h" 
#include "AABB.h"

class AnimatedSprite
{
private:
	AnimationSpriteSheet const*	mSpriteSheet;

public:
	Animator					mAnimator;
	uint						mFrame;  

public:
	AnimatedSprite() = default; 
	AnimatedSprite(AnimationSpriteSheet const* spriteSheet);    
	void Render(Surface8* screen, int const x, int const y) const;

	void SetSpriteSheet(AnimationSpriteSheet const* spriteSheet)
	{
		mSpriteSheet = spriteSheet;
		mAnimator = Animator(this);  
	}

	inline Animation const* GetAnimation(int const idx) const { return &mSpriteSheet->mAnimSet.anims[idx]; }  
	inline AnimationSpriteSheet const* GetSpriteSheet() const { return mSpriteSheet; }

	uint8_t GetPixel(int2 coord) const
	{
		if (AABB::Detect(coord, mSpriteSheet->mFrameWidth - 1, 0, 0, mSpriteSheet->height - 1))
		{
			return mSpriteSheet->palette->indices[mSpriteSheet->pixels[mFrame * mSpriteSheet->mFrameWidth + coord.x + coord.y * mSpriteSheet->width]];
		}
	}
};

