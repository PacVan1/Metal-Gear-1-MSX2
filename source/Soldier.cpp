#include "precomp.h"
#include "Soldier.h"

#include "Tilemap.h" 
#include "Projectile.h"
#include "Game.h"
#include "Soldiers.h"

void Soldier::InitSoldierTypes()
{
	typeData[SOLDIER_TYPES_GRAY].palette	= ColorPalette8("assets/color_palettes/gray_soldier.cpalette");
	typeData[SOLDIER_TYPES_GRAY].alertLevel	= Soldiers::alertLevels::LOW;
	typeData[SOLDIER_TYPES_RED ].palette	= ColorPalette8("assets/color_palettes/damaged.cpalette");
	typeData[SOLDIER_TYPES_RED ].alertLevel	= Soldiers::alertLevels::HIGH;
}

void Soldier::SetType(int const t)
{
	type = t; 
	spriteSheet.sprite.palette = &typeData[t].palette; 
}

Soldier::Soldier() :
	Enemy(spriteSheet, 2) 
{
	InitState();
	bbox.group = AABB::ENEMY; 
	speed = 0.08f;
	maxHealth = 10;
	ResetHealth();
}

void Soldier::Update(float const dt) 
{
	display.animator.Play(dt); 

	switch (state)
	{
	case SOLDIER_STATES_IDLE:			IdleState();		break;
	case SOLDIER_STATES_PATROL:			PatrolState(dt);	break;
	case SOLDIER_STATES_PURSUE:			PursueState(dt);	break;
	case SOLDIER_STATES_STUNNED:		StunnedState(dt);	break;
	case SOLDIER_STATES_SHOOT:			ShootState();		break;
	case SOLDIER_STATES_REALIZATION:	RealizationState(); break;
	case SOLDIER_STATES_SPOTTED:		SpottedState();		break;
	default: break; 
	}
}

void Soldier::SetAnimationState() 
{
	animState = STATE_TO_ANIM_STATE[state]; 
}

void Soldier::Damage(int const damage)
{
	health -= damage;
	if (health <= 0)
	{
		Destroy(); 
	}
	SetState(SOLDIER_STATES_STUNNED); 
	SetAnimation(); 
}

void Soldier::Alert()
{
	SetState(SOLDIER_STATES_PURSUE);
	DecideCardinal();
	SetAnimation();
}

void Soldier::Reset() 
{
	InitState();
	ResetHealth();  
}

void Soldier::TurnRandomSide()
{
	facing = static_cast<cardinals>(RandomUInt() % cardinals::COUNT);
	SetAnimationState(); 
	SetAnimation(); 
}

void Soldier::IdleState()
{
	if (TargetInLine())
	{
		SetState(SOLDIER_STATES_REALIZATION);
		return;
	}

	if (turnTimer.Elapsed()) 
	{
		turns++;
		turnTimer.Reset();
		if (turns >= TURNS) 
		{
			turns = 0;
			SetState(SOLDIER_STATES_PATROL); 
			SetAnimation();
			sequencer.Continue();
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
		shootTimer.Reset();
		SetState(SOLDIER_STATES_REALIZATION);
		return;
	}

	sequencer.Play(dt);
	if (sequencer.HasReachedEnd())
	{
		SetState(SOLDIER_STATES_IDLE);
		SetAnimation();
		return;
	}
	if (sequencer.HasReachedFlag()) 
	{ 
		SetState(SOLDIER_STATES_IDLE);
		SetAnimation();
	}
}

void Soldier::PursueState(float const dt)
{
	Pursue(dt);

	if (shootTimer.Elapsed())
	{
		standStillTimer.Reset();
		DecideCardinal(); 
		SetState(SOLDIER_STATES_SHOOT);
		SetAnimation();
	}
}

void Soldier::ShootState()
{
	if (standStillTimer.Elapsed())
	{
		if (!shot) // !shot
		{
			float2 dir = normalize(target->GetPosition() - GetPosition());
			Projectile::Launch(GetPosition(), normalize(dir) * 0.1f, 1, AABB::PLAYER);  
			shot = true;
		}
		else
		{
			shot = false;
			shootTimer.Reset();
			Alert(); 
		}
	}
}

void Soldier::SpottedState()
{
	if (spotAlarm.Elapsed())
	{
		Soldiers::SetAlertLevel(typeData[type].alertLevel);
	}
}

void Soldier::RealizationState()
{
	if (Soldiers::alertLevel >= Soldiers::alertLevels::SPOTTED) 
	{
		Alert();
	}
	else 
	{
		SetState(SOLDIER_STATES_SPOTTED); 
		SetAnimation();
		spotAlarm.Reset();
		Soldiers::SetAlertLevel(Soldiers::alertLevels::SPOTTED); 
	}
}

void Soldier::StunnedState(float const dt)
{
	if (!descend)
	{
		stunY -= 0.055f * dt;
		SetPosition(GetPosition() - float2(0.0f, 0.055f * dt));
		if (stunY <= -10.0f)
		{
			descend = true;
		}
	}
	else
	{
		stunY += 0.055f * dt; 
		SetPosition(GetPosition() + float2(0.0f, 0.055f * dt)); 
		if (stunY >= 0.0f)
		{
			stunY = 0.0f;
			descend = false;
			SetState(SOLDIER_STATES_REALIZATION);
		}
	}
}
