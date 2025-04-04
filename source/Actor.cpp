#include "precomp.h"
#include "Actor.h"

Actor::Actor(AnimationSpriteSheet& sheet, uint dirAnimCount) :
	GameObject({ sheet.frameWidth, sheet.sprite.height }, { -9, -25 }),
	bboxTile({ sheet.frameWidth - 4, 10 }, { -7, -3 }),
	display(sheet),
	dirAnimCount(dirAnimCount),
	sequencer(*this),
	speed(0.02f)
{
	SetPosition({ 120.0f, 50.0f });
}

void Actor::Render(Surface8* screen) const
{
	display.Render(screen, GetPositionInt().x + bbox.OFFSET.x, GetPositionInt().y + bbox.OFFSET.y);
	//bbox.Render(screen);
	//bboxTile.Render(screen); 
} 

bool Actor::DetectPixel(Actor const& other) const
{
	int2 dir = GetPositionInt() - other.GetPositionInt();

	// find the biggest span: 
	int const w = (display.sheet.frameWidth > other.display.sheet.frameWidth) ? display.sheet.frameWidth : other.display.sheet.frameWidth;
	int const h = (display.sheet.sprite.height > other.display.sheet.sprite.height) ? display.sheet.sprite.height : other.display.sheet.sprite.height;

	int idx = 0; 
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++, idx++)
	{
		if (display.GetPixel({ x, y }) != 0 && other.display.GetPixel(int2(x, y) - dir) != 0)
		{
			return true; 
		}
	}
	return false;
}

void Actor::Move(float const dt)
{   
	bboxTile.Move(facing, speed * dt);
	bbox.Update(bboxTile.GetCenter());  
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

bool Actor::DetectCollision(Actor const& other) const
{
	//if (AABB::Detect(bbox, other.bbox))
	//{
	//	if (AABB::Detect(bboxHead, other.bboxHead) || AABB::Detect(bboxBody, other.bboxBody))
	//	{
	//		return DetectPixel(other);  
	//	}
	//}
	return false;
}

void Actor::InitState()   
{
	facing = EAST; 
	SetState(0);   
	SetAnimation(); 
}
