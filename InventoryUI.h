#pragma once

#include "Inventory.h"

enum inventoryUIState : u8
{
	INVENTORY_UI_STATE_WEAPONS,
	INVENTORY_UI_STATE_EQUIPMENT
};

class InventoryUI
{
public:
	Inventory*	inv;
	int			state = INVENTORY_UI_STATE_WEAPONS; 
	int			selected; 

public:
	void Update(float const dt);  
	void Render(Surface8* screen) const;

private:
	void RenderWeapons(Surface8* screen) const;
	void RenderEquipment(Surface8* screen) const;
};

