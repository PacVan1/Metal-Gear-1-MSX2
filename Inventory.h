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
		CARD2,
		CARD3,
		CARD4,
		CARD5,
		CARD6,
		CARD7,
		CARD8,
		//GASMASK, 
		COUNT,
		EQUIPMENT_COUNT = COUNT - WEAPON_COUNT,
		AMMO
	};

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

