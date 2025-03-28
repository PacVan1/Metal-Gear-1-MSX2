#pragma once

#include "Item.h" 

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
		BINOCULAR = WEAPON_COUNT,
		COUNT,
		EQUIPMENT_COUNT = COUNT - WEAPON_COUNT,
		AMMO
	};

public:
	bool	unlocked[COUNT] = { false };
	Item*	items[COUNT]	= { nullptr };
	Item*	selectedWeapon; 
	Item*	selectedEquipment; 

public:
	Inventory();
	void PickUp(ItemObject& item);
};

