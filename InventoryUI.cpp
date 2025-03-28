#include "precomp.h"
#include "InventoryUI.h"

void InventoryUI::Render(Surface8* screen) const
{
	screen->Clear(0); // black

	switch (state)
	{
	case INVENTORY_UI_STATE_WEAPONS:	RenderWeapons(screen);		break;
	case INVENTORY_UI_STATE_EQUIPMENT:	RenderEquipment(screen);	break;
	}
}

void InventoryUI::RenderWeapons(Surface8* screen) const
{
	screen->Print("Weapon Select", NATIVE_SCREEN_WIDTH / 2 - 39, 10, 181);

	for (int i = 0; i < inv->unlockedWeaponsCount; i++)
	{
		screen->Box(10, 25 * i + 26, NATIVE_SCREEN_WIDTH / 2 - 5, 25 * i + 20 + 26, 181);
		screen->Print(inv->unlockedWeapons[i]->name, 15, 25 * i + 31, 181);
	}
}

void InventoryUI::RenderEquipment(Surface8* screen) const
{
	screen->Print("Equipment Select", NATIVE_SCREEN_WIDTH / 2, 10, 181);   

	for (int i = 0; i < inv->unlockedEquipmentCount; i++)
	{
		screen->Box(0, 25 * i, 100, 25 * i + 20, 181);
		screen->Print(inv->unlockedEquipment[i]->name, 1, i * 25 + 1, 181);
	}
}
