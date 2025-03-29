#pragma once

#include "Player.h" 

class PlayerUI
{
public:
	Player const* player; 

public:
	void Render(Surface8* screen) const;

private:
	void RenderHealthbar(Surface8* screen) const;
	void RenderItems(Surface8* screen) const; 
};

