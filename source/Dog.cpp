#include "precomp.h"
#include "Dog.h"

Dog::Dog() :
	Enemy(spriteSheet, 1)
{
	InitState();
	spriteSheet.palette = &palette;
	SetBehavior(PERSUE); 
	mSpeed = 0.07f;

	SetPosition({120.0f, 60.0f});
}

void Dog::Update(float const dt) 
{
	mDisplay.mAnimator.Play(dt); 

	switch (mBehavior) 
	{
	case IDLE: break;
	case PERSUE: Pursue(dt); break; 
	}
}

void Dog::SetAnimationState()
{ 
	mAnimState = STATE_TO_ANIM_STATE[mBehavior];   
}
