#pragma once

#include "Item.h" 
#include "TextPopup.h"

class Inventory
{
public:
	enum : u8
	{
		HANDGUN,
		LAND_MINE,
		WEAPON_COUNT
	};

	enum : u8
	{
		BINOCULARS = WEAPON_COUNT,
		GASMASK, 
		CARD1,
		CARD2,
		CARD3,
		CARD4,
		CARD5,
		CARD6,
		CARD7,
		CARD8,
		COUNT,
		EQUIPMENT_COUNT = COUNT - WEAPON_COUNT,
		AMMO
	};

public:
	inline static TextPopup textPopup; 

public:
	bool	unlocked[COUNT] = { false };
	Item*	items[COUNT]	= { nullptr };
	Item*	selectedWeapon; 
	Item*	selectedEquipment;
	Item*	unlockedWeapons[WEAPON_COUNT]		= { nullptr };
	Item*	unlockedEquipment[EQUIPMENT_COUNT]	= { nullptr };
	int		unlockedWeaponsCount	= 0; 
	int		unlockedEquipmentCount	= 0; 

public:
	Inventory();
	~Inventory(); 
	void PickUp(ItemObject& item);
	void Unlock(int itemType);  
};

class Handgun : public Item
{
public:
	float	speed = 0.4f;
	uint	damage = 100;

public:
			Handgun();
	void	Use() override;
};

class Landmine : public Item
{
public:
	Landmine();
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

