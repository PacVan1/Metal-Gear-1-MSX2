#include "precomp.h"
#include "Soldier.h"

#include "Tilemap.h" 
#include "Projectile.h"
#include "Game.h"
#include "Soldiers.h"

void Soldier::InitSoldierTypes()
{
	typeData[GRAY].palette		= ColorPalette8("assets/color_palettes/gray_soldier.cpalette");
	typeData[GRAY].alertLevel	= Soldiers::alertLevels::LOW;
	typeData[RED ].palette		= ColorPalette8("assets/color_palettes/damaged.cpalette"); 
	typeData[RED ].alertLevel	= Soldiers::alertLevels::HIGH; 
}

void Soldier::SetType(int type)
{
	Soldier::type = type; 
	spriteSheet.sprite.palette	= &typeData[type].palette; 
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
	case IDLE:			IdleState();		break;
	case PATROL:		PatrolState(dt);	break; 
	case PURSUE:		PersueState(dt);	break;
	case STUNNED:		StunnedState(dt);	break;
	case SHOOT:			ShootState();		break;
	case REALIZATION:	RealizationState(); break;
	case SPOTTED:		SpottedState();		break;
	}
}

void Soldier::SetAnimationState() 
{
	animState = STATE_TO_ANIM_STATE[state]; 
}

void Soldier::Damage(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		Destroy(); 
	}
	SetState(STUNNED);
	SetAnimation(); 
}

void Soldier::Alert()
{
	SetState(PURSUE); 
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
		SetState(REALIZATION); 
		return;
	}

	if (turnTimer.elapsed() > TURN_TIME) 
	{
		turns++;
		turnTimer.reset();
		if (turns >= TURNS) 
		{
			turns = 0;
			SetState(PATROL);
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
		shootTimer.reset(); 
		SetState(REALIZATION); 
		return;
	}

	sequencer.Play(dt);
	if (sequencer.HasReachedEnd())
	{
		printf("Soldier has reached end of path\n"); 
		SetState(IDLE);
		SetAnimation();
		return;
	}
	if (sequencer.HasReachedFlag()) 
	{ 
		SetState(IDLE);
		SetAnimation();
	}
}

void Soldier::PersueState(float const dt)
{
	Persue(dt);

	if (shootTimer.elapsed() >= SHOOT_TIME) 
	{
		standStillTimer.reset();
		DecideCardinal(); 
		SetState(SHOOT);
		SetAnimation();
	}
}

void Soldier::ShootState()
{
	if (standStillTimer.elapsed() >= STAND_STILL_TIME)
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
			shootTimer.reset();
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
		SetState(SPOTTED); 
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
			SetState(REALIZATION); 
		}
	}
}
