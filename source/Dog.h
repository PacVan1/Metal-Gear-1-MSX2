#pragma once

#include "Enemy.h" 

class Dog : public Enemy 
{
public:
	enum states
	{
		IDLE, PERSUE, COUNT
	};

private:
	inline static AnimationSpriteSheet spriteSheet = AnimationSpriteSheet("assets/spritesheets/dog/dog.animsheet"); 
	inline static ColorPalette8 palette = ColorPalette8("assets/color_palettes/dog.cpalette"); 
	inline static uint const STATE_TO_ANIM_STATE[states::COUNT] = { 1, 0 };   

public:
			Dog(); 
	void	Update(float const dt) override; 
	void	SetAnimationState() override;
	void	Damage(int damage) override {};
};

