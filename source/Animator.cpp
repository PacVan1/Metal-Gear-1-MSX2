#include "precomp.h"
#include "Animator.h"

#include "AnimatedSprite.h" 

Animator::Animator(AnimatedSprite* display) :
	mDisplay(display)
{
	SetAnimation(0);
} 

void Animator::Play(float const dt)
{
	if (!mAnim->hasDuration) 
	{
		return;
	}

	mFinished = false;
	mTime += dt; 
	if (static_cast<int>(mTime) >= mRate)     
	{
		mTime -= static_cast<float>(mRate); 
		mDisplay->mFrame++; 
		if (mDisplay->mFrame > mAnim->finalFrame) 
		{
			mDisplay->mFrame = mAnim->startFrame; 
			mFinished = true; 
		}
	}
}

void Animator::SetAnimation(int const idx)
{
	mAnimIdx = idx;   
	mAnim = mDisplay->GetAnimation(idx);   
	Reset();
}

void Animator::Reset()
{
	mFinished = false;
	mTime = 0.0f; 
	mDisplay->mFrame = mAnim->startFrame; 
}