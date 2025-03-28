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
		BINOCULARS = WEAPON_COUNT,
		CARD1,
		//GASMASK, 
		COUNT,
		EQUIPMENT_COUNT = COUNT - WEAPON_COUNT,
		AMMO
	};

public:
#if EVERYTHING_UNLOCKED
	bool	unlocked[COUNT] = { true };
#else
	bool	unlocked[COUNT] = { false };
#endif
	Item*	items[COUNT]	= { nullptr };
	Item*	selectedWeapon; 
	Item*	selectedEquipment; 

public:
	Inventory();
	~Inventory(); 
	void PickUp(ItemObject& item);
};

