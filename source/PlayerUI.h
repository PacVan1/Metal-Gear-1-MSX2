#pragma once

#include "Player.h"
#include "Inventory.h" 

class PlayerUI
{
public:
	Player const*		mPlayer;
	Inventory const*	mInventory; 

public:
			PlayerUI(Player const* player, Inventory const* inventory);   
	void	Render(Surface8* screen) const;

private:
	void	RenderHealthbar(Surface8* screen) const;
	void	RenderItems(Surface8* screen) const; 
};

