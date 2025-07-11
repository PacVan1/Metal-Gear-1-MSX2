#include "precomp.h"
#include "Player.h"

#include "Game.h"

Player::Player() :
	mDamageAlarm(DAMAGE_TIME),
	mColorCycleAlarm(COLOR_CYCLE_TIME),
	mPunchPoint(0.0f), 
	mBboxPunch(&mPunchPoint, 10.0f, -5.0f)
{
	mMaxHealth	= 20; 
	mSpeed		= 0.08f;
#if DEBUG_MODE
	mSpeed		= 0.2f;
#endif
	mBbox.mGroup		= COLLISION_GROUPS_PLAYER;
	mBboxTile.mGroup	= COLLISION_GROUPS_PLAYER;

	InitState();
	ResetHealth(); 
}

void Player::Update(float const dt)
{	
	mDisplay.mAnimator.Play(dt);

	// color cycling
	if (mDamaged) 
	{
		if (mColorCycleAlarm.Elapsed())
		{
			mColorCycleAlarm.Reset(); 
			mDamagedPalette.Cycle();
		}

		if (mDamageAlarm.Elapsed()) 
		{
			mDamageAlarm.Reset(); 
			mDamaged = false;
			SetPalette(&mNormalPalette);
		}
	}

	switch (mBehavior) 
	{
	case PLAYER_STATES_IDLE: 
		{
		// exit condition:
			 if (Game::sInput->IsKeyDown(CONTROLS_MOVE_EAST))	{ SetState(PLAYER_STATES_RUN, EAST); }
		else if (Game::sInput->IsKeyDown(CONTROLS_MOVE_NORTH))	{ SetState(PLAYER_STATES_RUN, NORTH); }
		else if (Game::sInput->IsKeyDown(CONTROLS_MOVE_WEST))	{ SetState(PLAYER_STATES_RUN, WEST); }
		else if (Game::sInput->IsKeyDown(CONTROLS_MOVE_SOUTH))	{ SetState(PLAYER_STATES_RUN, SOUTH); }
		
		if (Game::sInput->IsKeyReleased(CONTROLS_USE_WEAPON))		mInventory->UseWeapon();
		if (Game::sInput->IsKeyReleased(CONTROLS_USE_EQUIPMENT))	mInventory->UseEquipment(); 
		if (Game::sInput->IsKeyReleased(CONTROLS_TRIGGER_PUNCH))	Punch();
		if (Game::sInput->IsKeyReleased(CONTROLS_PICKUP_ITEM))
		{
			for (int i = 0; i < Game::GetScene().mItemCount; i++)
			{
				if (AABB::Detect(mBbox, Game::GetScene().mItems[i].GetBbox()))
				{
					mInventory->PickUp(Game::GetScene().mItems[i]);
				}
			}
		}

		break;
		}
	case PLAYER_STATES_RUN: 
		{
		// update state:
		Move(dt);
		
		// exit condition:
		if (Game::sInput->IsKeyDown(CONTROLS_MOVE_EAST))		{ SetFacing(EAST);	return; }
		if (Game::sInput->IsKeyDown(CONTROLS_MOVE_NORTH))	{ SetFacing(NORTH); return; }
		if (Game::sInput->IsKeyDown(CONTROLS_MOVE_WEST))		{ SetFacing(WEST);	return; }
		if (Game::sInput->IsKeyDown(CONTROLS_MOVE_SOUTH))	{ SetFacing(SOUTH); return; }

		SetBehavior(PLAYER_STATES_IDLE);

		break;
		}
	case PLAYER_STATES_SWIM: break;
	case PLAYER_STATES_PUNCH:
		{
		if (mDisplay.mAnimator.IsFinished()) 
		{
			SetBehavior(PLAYER_STATES_IDLE); 
		}
		break;
		}
	case PLAYER_STATES_CLIMB: break;
	case PLAYER_STATES_DEAD:  break;
	case PLAYER_STATES_DIVE:  break;
	default: break; 
	}
}

void Player::Render(Surface8* screen) const
{
	Actor::Render(screen);
	if (mBehavior == PLAYER_STATES_PUNCH) mBboxPunch.Render(screen); 
}

void Player::SetAnimationState()
{
	switch (mBehavior) 
	{
	case PLAYER_STATES_IDLE:	mAnimState = 0; if (IsArmed()) mAnimState++; break; // idle or armed idle
	case PLAYER_STATES_RUN: 	mAnimState = 2; if (IsArmed()) mAnimState++; break; // run or armed run  
	case PLAYER_STATES_SWIM:	mAnimState = 4; if (IsArmed()) mAnimState++; break; // swim or armed swim
	case PLAYER_STATES_PUNCH:	mAnimState = 6; break; 							  // punch 
	case PLAYER_STATES_CLIMB:	mAnimState = 7; break;							  // climb
	case PLAYER_STATES_DEAD:	mAnimState = 8; break;							  // dead
	case PLAYER_STATES_DIVE:	mAnimState = 9;									  // dive
	default: break;
	}
}

void Player::SetInventory(Inventory* inventory)
{
	mInventory = inventory; 
}

void Player::SetSpriteSheet(AnimationSpriteSheet& spriteSheet, int const dirAnimCount)
{
	Actor::SetSpriteSheet(spriteSheet, dirAnimCount);
	mSpriteSheet = &spriteSheet; 
}

void Player::SetPalette(ColorPalette8 const* colorPalette)
{
	mSpriteSheet->palette = colorPalette; 
}

void Player::Damage(int damage)
{
	mHealth -= damage; 

	if (mHealth <= 0)
	{
		Destroy(); 
	}
	else
	{
		mColorCycleAlarm.Reset();
		mDamageAlarm.Reset(); 
		mDamaged = true;
		SetPalette(&mDamagedPalette);  
	}
}

void Player::Destroy()
{
	mDestroyed = true;
	SetBehavior(PLAYER_STATES_DEAD);
	Game::EnterGameOverState();
}

void Player::Punch()
{
	SetBehavior(PLAYER_STATES_PUNCH); 

	// TODO make pixel perfect:

	mPunchPoint = mPosF + PUNCH_OFFSET + float2(4.0f, 3.0f) * cardinalToFloat2(mFacing); 
	mBboxPunch.Update();

	for (int i = 0; i < Game::sSoldiers.mPool.SIZE; i++)
	{
		if (!Game::sSoldiers.mPool.IsActive(i) || Game::sSoldiers.mPool[i].mDestroyed) continue; 
		//if (AABB::Detect(mBboxPunch, Game::sSoldiers.mPool[i].GetBbox()))  
		//{
		//	Game::sSoldiers.mPool[i].Damage(2);  
		//}
		if (Game::sSoldiers.mPool[i].DetectPixel(Game::sPlayer, Game::sPlayer.mBboxPunch))
		{
			Game::sSoldiers.mPool[i].Damage(2);
		}
	}
}