#pragma once

#include "Actor.h"
#include "Inventory.h"

float constexpr COLOR_CYCLE_TIME	= 0.2f;
float constexpr DAMAGE_TIME			= 1.2f;
float2 const	PUNCH_OFFSET		= float2(0.0f, -15.0f); 

enum playerStates : uint8_t
{
	PLAYER_STATES_IDLE,
	PLAYER_STATES_RUN,
	PLAYER_STATES_SWIM,
	PLAYER_STATES_PUNCH,
	PLAYER_STATES_CLIMB,
	PLAYER_STATES_DEAD,
	PLAYER_STATES_DIVE,
	PLAYER_STATES_COUNT, 
};

class Player final : public Actor 
{
public:
	ColorPalette8			mNormalPalette;
	ColorPalette8			mDamagedPalette; 

private:
	AnimationSpriteSheet*	mSpriteSheet; 
	Inventory*				mInventory; 
	AABB					mBboxPunch;  
	Alarm					mDamageAlarm;
	Alarm					mColorCycleAlarm; 
	float2					mPunchPoint;
	bool					mDamaged;

public:
				Player();
	void		Update(float const dt) override;
	void		Render(Surface8* screen) const override;

	void		SetAnimationState() override;
	void		SetInventory(Inventory* inventory);
	void		SetSpriteSheet(AnimationSpriteSheet& spriteSheet, int const dirAnimCount) override;
	void		SetPalette(ColorPalette8 const* colorPalette);

	void		Damage(int damage) override;
	void		Destroy() override; 
	void		Punch(); 
	inline bool	IsArmed() const { return mInventory && mInventory->mSelectedWeapon; }
};

