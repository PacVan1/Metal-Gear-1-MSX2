#pragma once

#include "Actor.h"
#include "Inventory.h"

class Player : public Actor 
{
public:
	enum states
	{
		IDLE, RUN, SWIM, PUNCH, CLIMB, DEAD, DIVE, COUNT  
	};

private:
	inline static AnimationSpriteSheet spriteSheet	= AnimationSpriteSheet("assets/spritesheets/snake_v2/snake.animsheet");
	inline static ColorPalette8 normalPalette		= ColorPalette8("assets/color_palettes/snake.cpalette");  
	inline static ColorPalette8 damagedPalette		= ColorPalette8("assets/color_palettes/damaged.cpalette");  

public:
	Inventory	inventory; 
	Timer		damageTimer;
	Timer		colorCycle; 
	bool		damaged;
	AABB		bboxPunch; 

public:
				Player(); 
	void		Update(float const dt) override; 
	void		SetAnimationState() override;
	void		Damage(int damage) override;
	void		Destroy() override; 
	void		Punch(); 
	bool		IsArmed() const { return inventory.selectedWeapon; }  
};

