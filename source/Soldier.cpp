#include "precomp.h"
#include "Soldier.h"

#include "Game.h"
#include "Projectile.h"
#include "SoldierTypes.h" 
#include "Soldiers.h"

Soldier::Soldier() :
	Enemy(Game::sSoldierTypes.mSpriteSheet, 2),
	mTurnAlarm(TURN_TIME),
	mShootAlarm(SHOOT_TIME),
	mStandStillAlarm(STAND_STILL_TIME),
	mSpotAlarm(SPOT_TIME),
	mStunY(0.0f),
	mTurns(0),
	mShot(false),
	mDescend(false)
{
	// TODO make this soldier type dependent:
	mSpeed		= 0.05f;
	mMaxHealth	= 10;  

	mBbox.mGroup		= COLLISION_GROUPS_ENEMY;
	mBboxTile.mGroup	= COLLISION_GROUPS_ENEMY;
	InitState();
	ResetHealth(); 
}

void Soldier::Update(float const dt) 
{
	mDisplay.mAnimator.Play(dt); 

	switch (mBehavior)
	{
	case SOLDIER_BEHAVIOR_IDLE:			IdleState();		break;
	case SOLDIER_BEHAVIOR_PATROL:		PatrolState(dt);	break;
	case SOLDIER_BEHAVIOR_PURSUE:		PursueState(dt);	break;
	case SOLDIER_BEHAVIOR_STUNNED:		StunnedState(dt);	break;
	case SOLDIER_BEHAVIOR_SHOOT:		ShootState();		break;
	case SOLDIER_BEHAVIOR_REALIZATION:	RealizationState(); break;
	case SOLDIER_BEHAVIOR_SPOTTED:		SpottedState();		break;
	default: break; 
	}
}

void Soldier::SetAnimationState() 
{
	mAnimState = Game::sSoldierTypes.mBehaviorToAnimState[mBehavior];  
}

void Soldier::Damage(int const damage)
{
	mHealth -= damage;
	if (mHealth <= 0) 
	{
		Destroy();
	}
	else
	{
		if (mBehavior != SOLDIER_BEHAVIOR_SPOTTED) SetBehavior(SOLDIER_BEHAVIOR_STUNNED);
	}
}

void Soldier::Alert()
{
	SetState(SOLDIER_BEHAVIOR_PURSUE, DecideFacing());
	mShootAlarm.Reset(); 
}

void Soldier::Destroy()
{
	mDestroyed = true; 
	if (Game::sSoldiers.AreDead())  
	{
		Game::sSoldiers.SetAlertLevel(ALERT_LEVELS_OFF); 
	}
}

void Soldier::Reset() 
{
	SetState(SOLDIER_BEHAVIOR_IDLE, EAST);  
	ResetHealth();
	mTurns = 0;
	mTurnAlarm.Reset();
}

void Soldier::TurnRandomSide()
{
	SetFacing(randomCardinal()); 
}

void Soldier::IdleState()
{
	if (TargetInLine())
	{
		SetBehavior(SOLDIER_BEHAVIOR_REALIZATION);
		return;
	}

	if (mTurnAlarm.Elapsed()) 
	{
		mTurns++;
		mTurnAlarm.Reset();
		if (mTurns >= TURNS)
		{
			mTurns = 0;
			SetBehavior(SOLDIER_BEHAVIOR_PATROL);
			mSequencer.Continue();
		}
		else
		{
			TurnRandomSide();
		}
	}
}

void Soldier::PatrolState(float const dt)
{
	if (TargetInLine())
	{
		SetBehavior(SOLDIER_BEHAVIOR_REALIZATION);
	}
	else
	{
		mSequencer.Play(dt);
		if (mSequencer.HasReachedFlag()) 
		{ 
			SetBehavior(SOLDIER_BEHAVIOR_IDLE);
		}
	}
}

void Soldier::PursueState(float const dt)
{
	Pursue(dt);

	if (mShootAlarm.Elapsed())
	{
		mStandStillAlarm.Reset();
		SetState(SOLDIER_BEHAVIOR_SHOOT, DecideFacing()); 
	}
}

void Soldier::ShootState()
{
	if (mStandStillAlarm.Elapsed())
	{
		if (!mShot)
		{
			float2 const from = GUN_POSITION + GetPosition() + cardinalToFloat2(mFacing) * GUN_OFFSET;
			float2 const dir = normalize(Game::sPlayer.GetPosition() - from); 
			Game::sProjectiles.Launch(from, normalize(dir) * 0.3f, 1, COLLISION_GROUPS_PLAYER);
			mShot = true;
		}
		else
		{
			mShot = false;
			mShootAlarm.Reset();
			Alert(); 
		}
	}
}

void Soldier::SpottedState()
{
	if (mSpotAlarm.Elapsed()) 
	{
		Game::sSoldiers.SetAlertLevel(); 
	}
}

void Soldier::RealizationState()
{
	if (Game::sSoldiers.GetAlertLevel() >= ALERT_LEVELS_SPOTTED)
	{
		Alert();
	}
	else 
	{
		SetBehavior(SOLDIER_BEHAVIOR_SPOTTED);
		Game::sSoldiers.SetAlertLevel(ALERT_LEVELS_SPOTTED);

		mSpotAlarm.Reset();
		Game::sSoldiers.mAlertPopup.SetPosition(GetPositionInt() + ALERT_POPUP_OFFSET); 
	}
}

void Soldier::StunnedState(float const dt)
{
	if (!mDescend) 
	{
		mStunY -= STUN_SPEED * dt;
		SetPosition(GetPosition() - float2(0.0f, STUN_SPEED * dt));
		if (mStunY <= -STUN_HEIGHT) 
		{
			mDescend = true;
		}
	}
	else
	{
		mStunY += STUN_SPEED * dt;
		SetPosition(GetPosition() + float2(0.0f, STUN_SPEED * dt));
		if (mStunY >= 0.0f)
		{
			mStunY = 0.0f;
			mDescend = false; 
			SetBehavior(SOLDIER_BEHAVIOR_REALIZATION);
		}
	}
}
