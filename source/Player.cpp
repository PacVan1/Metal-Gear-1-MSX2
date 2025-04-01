#include "precomp.h"
#include "Player.h"

Player::Player() :
	Actor(spriteSheet, 7),
	bboxPunch(10.0f, -5.0f) 
{
	InitState();
	spriteSheet.sprite.palette = &normalPalette;
	maxHealth = 20;
	ResetHealth(); 
	bbox.group = AABB::PLAYER; 
}

bool previ;
bool previ2;
bool previ3; 
void Player::Update(float const dt)
{	
	display.animator.Play(dt);

	// color cycling
	if (damaged)
	{
		if (colorCycle.elapsed() >= 0.2f)
		{
			colorCycle.reset();
			spriteSheet.sprite.palette->Cycle();
		}

		if (damageTimer.elapsed() >= 1.2f)
		{
			damageTimer.reset();
			damaged = false;
			spriteSheet.sprite.palette = &normalPalette; 
		}
	}

	switch (state)
	{
	case IDLE:
		{
		// exit condition:
		if		(GetAsyncKeyState(MOVE_EAST))	{ SetState(RUN); facing = EAST; SetAnimation(); }
		else if (GetAsyncKeyState(MOVE_NORTH))	{ SetState(RUN); facing = NORTH; SetAnimation(); }
		else if (GetAsyncKeyState(MOVE_WEST))	{ SetState(RUN); facing = WEST; SetAnimation(); }
		else if (GetAsyncKeyState(MOVE_SOUTH))	{ SetState(RUN); facing = SOUTH; SetAnimation(); }

		bool now = GetAsyncKeyState(USE_WEAPON);
		if (!now && previ2)
		{
			if (inventory.selectedWeapon)
			{
				inventory.selectedWeapon->Use();
			}
		}
		previ2 = now;

		now = GetAsyncKeyState(USE_EQUIPMENT); 
		if (!now && previ3)
		{
			if (inventory.selectedEquipment)
			{
				inventory.selectedEquipment->Use();
			}
		}
		previ3 = now;

		// punching
		now = GetAsyncKeyState(TRIGGER_PUNCH);
		if (!now && previ) Punch();
		previ = now;
		break;
		}
	case RUN:
		{
		// update state:
		Move(dt);

		// exit condition:
		if (GetAsyncKeyState(MOVE_EAST))	{ facing = EAST;	SetAnimation(); return; }
		if (GetAsyncKeyState(MOVE_NORTH))	{ facing = NORTH;	SetAnimation(); return; }
		if (GetAsyncKeyState(MOVE_WEST))	{ facing = WEST;	SetAnimation(); return; }
		if (GetAsyncKeyState(MOVE_SOUTH))	{ facing = SOUTH;	SetAnimation(); return; }

		bool now = GetAsyncKeyState(GLFW_KEY_SPACE); 
		if (!now && previ2)
		{
			previ2 = now;
			if (inventory.selectedWeapon)
			{
				inventory.selectedWeapon->Use();
			}
			return;
		}
		previ2 = now;

		// punching
		now = GetAsyncKeyState(TRIGGER_PUNCH);
		if (!now && previ)
		{
			previ = now; 
			Punch();
			return; 
		}
		previ = now;

		SetState(IDLE);
		SetAnimation();
		break;
		}
	case SWIM: break;
	case PUNCH:
		{
		if (display.animator.IsFinished())
		{
			SetState(IDLE);
			SetAnimation();
		}
		break;
		}
	case CLIMB: break;
	case DEAD:  break;
	case DIVE:  break;
	}
}

void Player::SetAnimationState()
{
	switch (state)
	{
	case IDLE:	animState = 0; if (IsArmed()) animState++; break; // idle or armed idle
	case RUN:	animState = 2; if (IsArmed()) animState++; break; // run or armed run  
	case SWIM:	animState = 4; if (IsArmed()) animState++; break; // swim or armed swim
	case PUNCH:	animState = 6; break; 							  // punch 
	case CLIMB: animState = 7; break;							  // climb
	case DEAD:  animState = 8; break;							  // dead
	case DIVE:  animState = 9;									  // dive
	}
}

void Player::Damage(int damage)
{
	health -= damage;

	if (health <= 0)
	{
		Destroy(); 
	}
	else
	{
		colorCycle.reset(); 
		damageTimer.reset(); 
		damaged = true;
		spriteSheet.sprite.palette = &damagedPalette; 
	}
}

void Player::Destroy()
{
	SetState(DEAD);
	SetAnimation(); 
}

#include "Soldiers.h"

void Player::Punch()
{
	SetState(PUNCH);
	SetAnimation();

	// place bbox in correct spot and do collision test:
	// TODO make pixel perfect:
	bboxPunch.Update(GetPosition());
	for (int i = 0; i < Soldiers::pool.SIZE; i++) 
	{
		if (!Soldiers::pool.IsActive(i) || Soldiers::pool[i].destroyed) continue;
		if (AABB::Detect(bboxPunch, Soldiers::pool[i].bbox))
		{
			Soldiers::Damage(i, 2);
		}
	}
}
