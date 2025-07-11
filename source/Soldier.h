#pragma once

#include "Enemy.h"

float constexpr TURN_TIME			= 1.0f; 
float constexpr SHOOT_TIME			= 1.0f; 
float constexpr STAND_STILL_TIME	= 1.0f; 
float constexpr SPOT_TIME			= 1.0f;
float constexpr STUN_SPEED			= 0.055f;
float constexpr STUN_HEIGHT			= 10.0f;
int constexpr	TURNS				= 4;
int2 const		ALERT_POPUP_OFFSET	= int2(10, -38);
float2 const	GUN_POSITION		= float2(0.0f, -15.0f); 
float2 const	GUN_OFFSET			= float2(7.0f, 5.0f); 

enum soldierBehavior : uint8_t 
{
	SOLDIER_BEHAVIOR_IDLE,
	SOLDIER_BEHAVIOR_PATROL,
	SOLDIER_BEHAVIOR_PURSUE,
	SOLDIER_BEHAVIOR_STUNNED,
	SOLDIER_BEHAVIOR_SHOOT,
	SOLDIER_BEHAVIOR_REALIZATION,
	SOLDIER_BEHAVIOR_SPOTTED,
	SOLDIER_BEHAVIOR_COUNT,
};

struct SoldierTypeData
{
	ColorPalette8	mPalette;
	float			mSpeed;
	int				mMaxHealth; 
	int				mAlertLevel;  
};

class Soldier final : public Enemy  
{
private:
	Alarm	mTurnAlarm;
	Alarm	mShootAlarm;
	Alarm	mStandStillAlarm;
	Alarm	mSpotAlarm; 

	float	mStunY;
	int		mTurns;
	bool	mShot;
	bool	mDescend; 

public:
			Soldier();  
	void	Update(float const dt) override; 
	void	SetAnimationState() override;
	void	Damage(int const damage) override;
	void	Destroy() override;
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
