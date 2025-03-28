#include "precomp.h"
#include "Inventory.h"

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
	items[CARD1]		= new Card(CARD1);

#if EVERYTHING_UNLOCKED
	// default selected items: 
	selectedWeapon		= items[HANDGUN]; 
	selectedEquipment	= items[CARD1];
#endif
}

Inventory::~Inventory()
{
	delete[] items; 
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
		unlocked[item.type] = true;
		selectedWeapon = items[item.type]; // TODO TEMPORARY
	}
	else // unlocked
	{
		items[item.type]->Stack(1);
	}
	item.pickedUp = true; 
}
