#include "precomp.h"
#include "Inventory.h"

#include "Gun.h" 

Inventory::Inventory() 
{
	items[HANDGUN]		= new Gun();
	//items[BINOCULAR]	= Binocular();
	//items[MINE]			= Mine();
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
