#pragma once

#include "Enemy.h"
#include "Player.h" 
#include "ObjectPool.h" 

struct SoldierTypeData
{
	ColorPalette8	palette;
	int				alertLevel; 
};

class Soldier : public Enemy 
{
public:
	enum types : u8
	{
		GRAY,
		RED,
		TYPE_COUNT
	};
	enum states : u8
	{
		IDLE,
		PATROL,
		PURSUE,
		STUNNED,
		SHOOT,
		SPOTTED,
		STATE_COUNT 
	};

public:
	inline static float constexpr	TURN_TIME			= 1.0f; // a second
	inline static float constexpr	SHOOT_TIME			= 2.0f; // two seconds
	inline static float constexpr	STAND_STILL_TIME	= 0.3f; 
	inline static uint constexpr	TURNS				= 4;	// turns before soldier starts patrolling 

private: // experimental: 
	inline static AnimationSpriteSheet spriteSheet = AnimationSpriteSheet("assets/spritesheets/soldier/soldier.animsheet");  
	inline static uint const STATE_TO_ANIM_STATE[STATE_COUNT] = { 0, 1, 1, 0, 0, 0 };  
	inline static SoldierTypeData typeData[TYPE_COUNT];
	inline static int type = GRAY; 

public:
	static void InitSoldierTypes();
	static void SetType(int type);  

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
	void	Alert(); 
	void	Reset();

private:
	void	TurnRandomSide();
	
	void	IdleState();
	void	PatrolState(float const dt); 
	void	PersueState(float const dt);
	void	ShootState();
	void	StunnedState(float const dt); 
};
