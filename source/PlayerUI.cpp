#include "precomp.h"
#include "PlayerUI.h"

#include "Game.h" 

PlayerUI::PlayerUI(Player const* player, Inventory const* inventory) :
	mPlayer(player),
	mInventory(inventory) 
{}

void PlayerUI::Render(Surface8* screen) const
{
	screen->Bar(0, NATIVE_SCREEN_HEIGHT - 20, NATIVE_SCREEN_WIDTH - 1, NATIVE_SCREEN_HEIGHT - 1, 0);

	RenderHealthbar(screen);
	RenderItems(screen); 
}

void PlayerUI::RenderHealthbar(Surface8* screen) const
{
	screen->Print("Life", 10, NATIVE_SCREEN_HEIGHT - 17, 181); 
	screen->Box(40, NATIVE_SCREEN_HEIGHT - 19, 40 + 80 + 2, NATIVE_SCREEN_HEIGHT - 10, 181);
	screen->Bar(41, NATIVE_SCREEN_HEIGHT - 18, 41 + 80 * (mPlayer->mHealth / static_cast<float>(mPlayer->mMaxHealth)), NATIVE_SCREEN_HEIGHT - 11, 40);
}

void PlayerUI::RenderItems(Surface8* screen) const
{
	// weapons:
	screen->Box(140, NATIVE_SCREEN_HEIGHT - 19, 186, NATIVE_SCREEN_HEIGHT - 2, 181);
	if (mInventory->mSelectedWeapon)
	{
		mInventory->mSelectedWeapon->Render(screen, 141, NATIVE_SCREEN_HEIGHT - 18);

		if (mInventory->mSelectedWeapon->stackable)
		{
			screen->Print(mInventory->mSelectedWeapon->strCount, 141 + 32, NATIVE_SCREEN_HEIGHT - 18 + 8, 181);
		}
	}

	// equipment:
	screen->Box(210, NATIVE_SCREEN_HEIGHT - 19, 233, NATIVE_SCREEN_HEIGHT - 2, 181);
	if (mInventory->mSelectedEquipment)
	{
		mInventory->mSelectedEquipment->Render(screen, 211, NATIVE_SCREEN_HEIGHT - 18);

		if (mInventory->mSelectedEquipment->stackable)
		{
			screen->Print(mInventory->mSelectedEquipment->strCount, 211 + 16, NATIVE_SCREEN_HEIGHT - 18 + 8, 181);
		}
	}
}
