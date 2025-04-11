#include "precomp.h"
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(AnimationSpriteSheet const* spriteSheet) : 
	mSpriteSheet(spriteSheet),
	mAnimator(this), 
	mFrame(0)
{}

void AnimatedSprite::Render(Surface8* screen, int const x, int const y) const
{
	mSpriteSheet->RenderFrame(screen, x, y, mFrame);  
}
