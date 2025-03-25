#include "precomp.h"
#include "Actor.h"

Actor::Actor(AnimationSpriteSheet& sheet, uint dirAnimCount) :
	GameObject({ sheet.frameWidth, sheet.sprite.height }, { -9, -25 }),
	bboxTile({ sheet.frameWidth - 4, 10 }, { -7, -3 }),
	display(sheet),
	dirAnimCount(dirAnimCount),
	sequencer(*this),
	speed(0.2f)
{
	SetPosition({ 120.0f, 50.0f });
}

void Actor::Render(Surface8* screen) const
{
	display.Render(screen, bbox.iPos.x + bbox.OFFSET.x, bbox.iPos.y + bbox.OFFSET.y);    
	bbox.Render(screen);
	bboxTile.Render(screen); 
} 

void Actor::Move(float const dt) 
{   
	bboxTile.Move(facing, speed * dt);
	bbox.Update(bboxTile.fPos); 
}
void Actor::SetPosition(float2 const position)
{
	bbox.Update(position);
	bboxTile.Update(position);  
}

void Actor::SetAnimation() 
{ 
	uint animIdx = GetAnimationIdx(); 
	if (animIdx != display.animator.animIdx) // not already playing the animation 
	{
		display.animator.SetAnimation(animIdx); 
	}
}

uint Actor::GetAnimationIdx() const 
{
	if (animState + 1 > dirAnimCount) 
	{
		// get whatever animation it is after the bundled animation states:
		return dirAnimCount * 4 + (animState - dirAnimCount); 
	}
	// get the animation state and in the correct direction:
	return facing * dirAnimCount + animState;
}

void Actor::SetState(uint state) 
{
	this->state = state;  
	SetAnimationState(); 
}

void Actor::InitState()   
{
	facing = EAST; 
	SetState(0);   
	SetAnimation(); 
}
