#pragma once

#include "Item.h"

class Gun : public Item
{
public:
	float	speed	= 0.4f; 
	uint	ammo	= 99;
	uint	damage	= 100;

public:
			Gun();
	void	Use() override;
};

