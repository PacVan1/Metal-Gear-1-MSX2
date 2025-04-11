#pragma once

#include "Item.h" 

enum itemTypes : uint8_t
{
	ITEM_TYPES_HANDGUN, 
	ITEM_TYPES_LAND_MINE,
	ITEM_TYPES_WEAPON_COUNT,

	ITEM_TYPES_BINOCULARS = ITEM_TYPES_WEAPON_COUNT,
	ITEM_TYPES_GAS_MASK,
	ITEM_TYPES_CARD1,
	ITEM_TYPES_CARD2,
	ITEM_TYPES_CARD3,
	ITEM_TYPES_CARD4,
	ITEM_TYPES_CARD5,
	ITEM_TYPES_CARD6,
	ITEM_TYPES_CARD7,
	ITEM_TYPES_CARD8,

	ITEM_TYPES_COUNT,
	ITEM_TYPES_EQUIPMENT_COUNT = ITEM_TYPES_COUNT - ITEM_TYPES_WEAPON_COUNT,
	ITEM_TYPES_AMMO,
	ITEM_TYPES_NONE
};

class Inventory
{
public:
	bool	mUnlocked[ITEM_TYPES_COUNT] = { false };
	Item*	mItems[ITEM_TYPES_COUNT]	= { nullptr };
	Item*	mSelectedWeapon; 
	Item*	mSelectedEquipment;
	Item*	mUnlockedWeapons[ITEM_TYPES_WEAPON_COUNT]		= { nullptr };
	Item*	mUnlockedEquipment[ITEM_TYPES_EQUIPMENT_COUNT]	= { nullptr };
	int		mUnlockedWeaponsCount	= 0; 
	int		mUnlockedEquipmentCount	= 0; 

public:
	Inventory();
	~Inventory(); 
	void PickUp(ItemObject& item);
	void Unlock(int itemType);
	void UseWeapon();
	void UseEquipment();
};

class Handgun : public Item
{
public:
	float2	mOffset = float2(5.0f, 3.0f);
	float	mSpeed	= 0.3f;
	int		mDamage = 100;
	int		mCollisionGroup = COLLISION_GROUPS_ENEMY; 

private:
	float2	mDir; 

public:
			Handgun();
	void	PickUp() override;
	void	Use() override;
};

class Landmine : public Item
{
public:
			Landmine();
	void	Use() override;
};

class Binoculars : public Item
{
public:
			Binoculars(); 
	void	Use() override;
};

class GasMask : public Item
{
// this item doesn't do anything
public:
	GasMask(); 
};

class Card : public Item
{
public:
	int level;

public:
			Card(int level);
	void	Use() override;
};

