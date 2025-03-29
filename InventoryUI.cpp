#include "precomp.h"
#include "InventoryUI.h"

bool prevup = false;
bool prevdown = false;
bool prevaccept = false; 
void InventoryUI::Update(float const dt)
{
	bool now = GetAsyncKeyState(INVENTORY_UP);  
	if (!now && prevup)
	{
		selected = (selected - 1 >= 0) ? selected - 1 : selected; 
	}
	prevup = now;

	now = GetAsyncKeyState(INVENTORY_DOWN);
	if (!now && prevdown)
	{
		selected = (selected + 1 <= Inventory::EQUIPMENT_COUNT - 1) ? selected + 1 : selected;
	}
	prevdown = now;

	now = GetAsyncKeyState(INVENTORY_SELECT); 
	if (!now && prevaccept)
	{
		inv->selectedEquipment = inv->unlockedEquipment[selected]; 
	}
	prevaccept = now;
}

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

	screen->Box(0, 25 * selected + 26, 10, 25 * selected + 36, 181); // cursor test
}

void InventoryUI::RenderEquipment(Surface8* screen) const
{
	screen->Print("Equipment Select", NATIVE_SCREEN_WIDTH / 2 - 48, 10, 181);

	for (int i = 0; i < inv->unlockedEquipmentCount; i++)
	{
		screen->Box(10, 25 * i + 26, NATIVE_SCREEN_WIDTH / 2 - 5, 25 * i + 20 + 26, 181);
		screen->Print(inv->unlockedEquipment[i]->name, 15, 25 * i + 31, 181);
	}

	screen->Box(0, 25 * selected + 26, 10, 25 * selected + 36, 181); // cursor test
}
