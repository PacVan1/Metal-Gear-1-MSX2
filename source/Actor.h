#pragma once

#include "GameObject.h"
#include "IDamageable.h" 
#include "AnimatedSprite.h" 
#include "Sequence.h"

class Actor : public GameObject, public IDamageable
{
public:
	AnimatedSprite	display;
	Sequencer		sequencer; 
	AABB			bboxTile; 
	int				facing;
	uint			state;  
	float			speed; 

protected:
	uint			dirAnimCount; 
	uint			animState; 

public:
					Actor(AnimationSpriteSheet& sheet, uint dirAnimCount);
	void			Render(Surface8* screen) const; 
	virtual void	Update(float const dt) = 0;    
	void			SetPosition(float2 const position);
	void			SetAnimation(); 
	void			SetState(uint state);

protected: 
	void			Move(float const dt);
	uint			GetAnimationIdx() const;  
	void			InitState();
	virtual void	SetAnimationState() = 0; 
};

