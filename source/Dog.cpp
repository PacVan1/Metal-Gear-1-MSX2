#include "precomp.h"
#include "Dog.h"

Dog::Dog() :
	Enemy(spriteSheet, 1)
{
	InitState();
	spriteSheet.sprite.palette = &palette;
	SetState(PERSUE); 

	speed = 0.07f;

	SetPosition({120.0f, 60.0f});
}

void Dog::Update(float const dt) 
{
	display.animator.Play(dt);

	switch (state)
	{
	case IDLE: break;
	case PERSUE: 
		// is the guard not facing the most optimal direction
		if (!oppositeFacing && facing != optimalFacing)
		{
			// try to go in the most optimal direction
			if (!bboxTile.DetectTilemap(optimalFacing, speed * dt))
			{
				facing = optimalFacing;
				SetAnimation();
			}
		}

		// is the guard hitting a wall OR reaching the target
		if (bboxTile.DetectTilemap(facing, speed * dt) ||
			CardinalToInt2(facing) * GetPositionInt() == CardinalToInt2(facing) * target->GetPositionInt())
		{
			DecideCardinal();
			SetAnimation();
		}

		Move(dt);
		break; 
	}
}

void Dog::SetAnimationState()
{ 
	animState = STATE_TO_ANIM_STATE[state]; 
}
