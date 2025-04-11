#include "precomp.h"
#include "InventoryUI.h"

#include "Game.h" 

InventoryUI::InventoryUI(Inventory* inventory) :
	mInventory(inventory),
	mState(INVENTORY_UI_STATES_WEAPONS),
	mSelected(0)
{}

void InventoryUI::Update(float const dt)
{
	switch (mState) 
	{
	case INVENTORY_UI_STATES_WEAPONS:
		{
		if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_UP))
		{
			mSelected = (mSelected - 1 >= 0) ? mSelected - 1 : mSelected;
		}
		if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_DOWN))
		{
			mSelected = (mSelected + 1 <= mInventory->mUnlockedWeaponsCount - 1) ? mSelected + 1 : mSelected;
		}
		if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_SELECT))
		{
			mInventory->mSelectedWeapon = mInventory->mUnlockedWeapons[mSelected]; 
		}
		break;
		}
	case INVENTORY_UI_STATES_EQUIPMENT:
		{
		if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_UP))
		{
			mSelected = (mSelected - 1 >= 0) ? mSelected - 1 : mSelected;
		}
		if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_DOWN))
		{
			mSelected = (mSelected + 1 <= mInventory->mUnlockedEquipmentCount - 1) ? mSelected + 1 : mSelected;
		}
		if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_SELECT))
		{
			mInventory->mSelectedEquipment = mInventory->mUnlockedEquipment[mSelected];
		}
		break;
		}
	default: break;
	}

	if (Game::sInput->IsKeyReleased(CONTROLS_INVENTORY_SWITCH))
	{
		mState = (mState == 0) ? mState = 1 : mState = 0;
		mSelected = 0; 
	}
}

void InventoryUI::Render(Surface8* screen) const
{
	screen->Clear(0); // blacks

	switch (mState) 
	{
	case INVENTORY_UI_STATES_WEAPONS:	RenderWeapons(screen);		break;
	case INVENTORY_UI_STATES_EQUIPMENT:	RenderEquipment(screen);	break;
	default: break; 
	}
}

void InventoryUI::RenderWeapons(Surface8* screen) const
{
	screen->Print("Weapon Select", NATIVE_SCREEN_WIDTH / 2 - 39, 10, 181);

	// render equipment:
	for (int i = 0; i < mInventory->mUnlockedWeaponsCount; i++)
	{
		mInventory->mUnlockedWeapons[i]->Render(screen, HORIZONTAL_ITEM_SPACING, VERTICAL_SPACING + ITEM_HEIGHT * i);
		screen->Print(mInventory->mUnlockedWeapons[i]->name, HORIZONTAL_ITEM_SPACING + WEAPON_WIDTH, VERTICAL_SPACING + i * ITEM_HEIGHT, 181);

		if (mInventory->mUnlockedWeapons[i]->stackable)
		{
			screen->Print(mInventory->mUnlockedWeapons[i]->strCount, HORIZONTAL_ITEM_SPACING + HORIZONTAL_STACK_SPACING + 12, VERTICAL_SPACING + i * ITEM_HEIGHT + FONT_SIZE + 2, 181);
		}
	}

	// render cursor:
	if (mInventory->mUnlockedWeaponsCount > 0)
	{
		Item::spriteSheet.RenderFrame(screen, HORIZONTAL_ITEM_SPACING - CURSOR_SIZE, VERTICAL_SPACING + mSelected * ITEM_HEIGHT, 0);
	}
}

void InventoryUI::RenderEquipment(Surface8* screen) const
{
	screen->Print("Equipment Select", NATIVE_SCREEN_WIDTH / 2 - 48, 10, 181);

	// render equipment:
	for (int i = 0; i < mInventory->mUnlockedEquipmentCount; i++)
	{
		mInventory->mUnlockedEquipment[i]->Render(screen, HORIZONTAL_ITEM_SPACING, VERTICAL_SPACING + ITEM_HEIGHT * i);
		screen->Print(mInventory->mUnlockedEquipment[i]->name, HORIZONTAL_ITEM_SPACING + EQUIPMENT_WIDTH, VERTICAL_SPACING + i * ITEM_HEIGHT, 181);

		if (mInventory->mUnlockedEquipment[i]->stackable)
		{
			screen->Print(mInventory->mUnlockedEquipment[i]->strCount, HORIZONTAL_ITEM_SPACING + HORIZONTAL_STACK_SPACING, VERTICAL_SPACING + i * ITEM_HEIGHT + FONT_SIZE + 2, 181);
		}
	}

	// render cursor:
	if (mInventory->mUnlockedEquipmentCount > 0)
	{
		Item::spriteSheet.RenderFrame(screen, HORIZONTAL_ITEM_SPACING - CURSOR_SIZE, VERTICAL_SPACING + mSelected * ITEM_HEIGHT, 0);
	}
}
