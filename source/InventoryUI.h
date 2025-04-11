#pragma once

#include "Inventory.h"

int constexpr VERTICAL_SPACING			= 25;
int constexpr HORIZONTAL_ITEM_SPACING	= 28;
int constexpr HORIZONTAL_STACK_SPACING	= 46;
int constexpr ITEM_HEIGHT				= 16;
int constexpr EQUIPMENT_WIDTH			= 16;
int constexpr WEAPON_WIDTH				= 32;

int constexpr CURSOR_SIZE				= 16;

enum inventoryUIState : uint8_t 
{
	INVENTORY_UI_STATES_WEAPONS,
	INVENTORY_UI_STATES_EQUIPMENT
};

class InventoryUI
{
private:
	Inventory*	mInventory; 
	int			mState; 
	int			mSelected; 

public:
			InventoryUI(Inventory* inventory);  
	void	Update(float const dt);  
	void	Render(Surface8* screen) const;

private:
	void	RenderWeapons(Surface8* screen) const;
	void	RenderEquipment(Surface8* screen) const;
};

