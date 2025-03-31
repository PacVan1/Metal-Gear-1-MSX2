#pragma once

#include "Enemy.h"

#define TURN_TIME			1.0f // in seconds
#define SHOOT_TIME			2.0f // in seconds
#define STAND_STILL_TIME	0.3f // in seconds
#define SPOT_TIME			2.0f // in seconds
#define TURNS				4	 // turns before soldier starts patrolling

enum soldierTypes : u8
{
	SOLDIER_TYPES_GRAY,
	SOLDIER_TYPES_RED,
	SOLDIER_TYPES_COUNT
};

enum soldierStates : u8
{
	SOLDIER_STATES_IDLE,
	SOLDIER_STATES_PATROL,
	SOLDIER_STATES_PURSUE,
	SOLDIER_STATES_STUNNED,
	SOLDIER_STATES_SHOOT,
	SOLDIER_STATES_REALIZATION,
	SOLDIER_STATES_SPOTTED,
	SOLDIER_STATES_COUNT
};

struct SoldierTypeData
{
	ColorPalette8	palette;
	int				alertLevel; 
};

class Soldier : public Enemy 
{
private: // experimental: 
	inline static AnimationSpriteSheet spriteSheet = AnimationSpriteSheet("assets/spritesheets/soldier/soldier.animsheet");  
	inline static uint const STATE_TO_ANIM_STATE[SOLDIER_STATES_COUNT] = { 0, 1, 1, 0, 0, 0, 0 };  
	inline static SoldierTypeData typeData[SOLDIER_TYPES_COUNT];
	inline static int	type; 
	inline static Alarm spotAlarm = Alarm(SPOT_TIME);

public:
	static void InitSoldierTypes();
	static void SetType(int const type);

private:
	Alarm	turnTimer		= Alarm(TURN_TIME); 
	Alarm	shootTimer		= Alarm(SHOOT_TIME);
	Alarm	standStillTimer = Alarm(STAND_STILL_TIME);

	uint	turns	= 0;
	float	stunY	= 0.0f;
	bool	shot	= false;
	bool	descend = false; 

public:
			Soldier();  
	void	Update(float const dt) override; 
	void	SetAnimationState() override;
	void	Damage(int const damage) override;
	void	Alert(); 
	void	Reset();

private:
	void	TurnRandomSide();
	
	void	IdleState();
	void	PatrolState(float const dt); 
	void	PursueState(float const dt);
	void	StunnedState(float const dt); 
	void	ShootState();
	void	RealizationState(); 
	void	SpottedState();
};
