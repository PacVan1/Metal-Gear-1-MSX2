#include "precomp.h"
#include "PlayerUI.h"

#include "Game.h" 

void PlayerUI::Render(Surface8* screen) const
{
	screen->Bar(0, NATIVE_SCREEN_HEIGHT - 20, NATIVE_SCREEN_WIDTH - 1, NATIVE_SCREEN_HEIGHT - 1, 0);

	RenderHealthbar(screen);
	RenderItems(screen); 
}

void PlayerUI::RenderHealthbar(Surface8* screen) const
{
	screen->Print("Life", 10, NATIVE_SCREEN_HEIGHT - 17, 181); 
	screen->Box(40, NATIVE_SCREEN_HEIGHT - 17, 40 + 80 + 2, NATIVE_SCREEN_HEIGHT - 4, 181);
	screen->Bar(41, NATIVE_SCREEN_HEIGHT - 16, 41 + 80 * (player->health / player->maxHealth), NATIVE_SCREEN_HEIGHT - 5, 40);
}

void PlayerUI::RenderItems(Surface8* screen) const
{
	screen->Print(player->inventory.selectedWeapon->name, 145, NATIVE_SCREEN_HEIGHT - 17, 181); 
	screen->Box(140, NATIVE_SCREEN_HEIGHT - 17, 200, NATIVE_SCREEN_HEIGHT - 4, 181);
	screen->Print(player->inventory.selectedEquipment->name, 215, NATIVE_SCREEN_HEIGHT - 17, 181);
	screen->Box(210, NATIVE_SCREEN_HEIGHT - 17, 246, NATIVE_SCREEN_HEIGHT - 4, 181);
}
