#include "precomp.h"
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(AnimationSpriteSheet& sheet) : 
	sheet(sheet), 
	animator(*this), 
	frame(0)
{}

void AnimatedSprite::Render(Surface8* screen, int x, int y) const
{
	sheet.Render(screen, x, y, frame);  
}
