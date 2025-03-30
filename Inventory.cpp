#include "precomp.h"
#include "Inventory.h"

#include "Game.h"
#include "Gun.h" 
#include "Binoculars.h"
#include "LandMine.h"

Inventory::Inventory() :
	selectedWeapon(nullptr),
	selectedEquipment(nullptr)
{
	items[HANDGUN]		= new Gun();
	items[LAND_MINE]	= new LandMine();
	items[BINOCULARS]	= new Binoculars();
	items[CARD1]		= new Card(1);
	items[CARD2]		= new Card(2);
	items[CARD3]		= new Card(3);
	items[CARD4]		= new Card(4);
	items[CARD5]		= new Card(5);
	items[CARD6]		= new Card(6);
	items[CARD7]		= new Card(7);
	items[CARD8]		= new Card(8);

#if EVERYTHING_UNLOCKED
	// default selected items: 
	for (int i = 0; i < COUNT; i++) { Unlock(i); items[i]->Stack(1); }

	selectedWeapon		= items[HANDGUN]; 
	selectedEquipment	= items[CARD1];
#endif
}

Inventory::~Inventory()
{
	for (int i = 0; i < COUNT; i++)
	{
		delete items[i];
	}
}

void Inventory::PickUp(ItemObject& item) 
{
	if (item.type == AMMO)
	{
		if (selectedWeapon)
		{
			selectedWeapon->Stack(items[item.type]->count);
		}
	}
	else if (!unlocked[item.type])
	{
		Unlock(item.type);  
	}
	else // unlocked
	{
		items[item.type]->Stack(1);
	}
	item.pickedUp = true;

	Game::SetTextPopup(item.GetPositionInt() + int2(10, -10), items[item.type]->name);
}

void Inventory::Unlock(int itemType)
{
	unlocked[itemType] = true;
	if (itemType < WEAPON_COUNT)
	{
		selectedWeapon = items[itemType];
		unlockedWeapons[unlockedWeaponsCount++] = items[itemType]; 
	}
	else
	{
		selectedEquipment = items[itemType];
		unlockedEquipment[unlockedEquipmentCount++] = items[itemType];
	}
}
