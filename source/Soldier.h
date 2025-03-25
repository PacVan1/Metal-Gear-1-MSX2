#pragma once

#include "Enemy.h"
#include "Player.h" 
#include "ObjectPool.h" 

class Soldier : public Enemy 
{
public:
	enum types
	{
		GRAY, RED, TYPE_COUNT
	};
	enum states
	{
		IDLE, PATROL, PERSUE, STUNNED, SHOOT, SPOTTED, STATE_COUNT 
	};

	enum alertStates
	{
		OFF, LOW, HIGH, ALERT_COUNT
	};

public:
	inline static int				alertState			= OFF; 

	inline static float constexpr	TURN_TIME			= 1.0f; // a second
	inline static float constexpr	SHOOT_TIME			= 2.0f; // two seconds
	inline static float constexpr	STAND_STILL_TIME	= 0.3f; 
	inline static uint constexpr	TURNS				= 4;	// turns before soldier starts patrolling 

private:
	inline static AnimationSpriteSheet spriteSheet = AnimationSpriteSheet("assets/spritesheets/soldier/soldier.animsheet");  
	inline static ColorPalette8 palettes[TYPE_COUNT] = // EXPERIMENTAL 
	{
			ColorPalette8("assets/color_palettes/gray_soldier.cpalette"),
			ColorPalette8("assets/color_palettes/damaged.cpalette")
	};
	inline static uint const STATE_TO_ANIM_STATE[STATE_COUNT] = { 0, 1, 1, 0, 0, 0 };
	inline static int type = GRAY; 

private:
	Timer	turnTimer;
	Timer	shootTimer;
	Timer	standStillTimer;

	uint	turns;
	bool	shot = false;
	float	stunY = 0.0f;
	bool	descend = false; 

public:
			Soldier();  
	void	Update(float const dt) override; 
	void	SetAnimationState() override;
	void	Damage(int damage) override;
	void	Reset();

private:
	void	TurnRandomSide();
	
	void	IdleState();
	void	PatrolState(float const dt); 
	void	PersueState(float const dt);
	void	ShootState();
	void	StunnedState(float const dt); 
};
