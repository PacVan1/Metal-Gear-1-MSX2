#pragma once

class Gun
{
public:
	float	speed = 0.4f; 
	uint	ammo = 99;
	uint	damage = 100;

public:
			Gun() = default; 
	void	Shoot(float2 from, float2 to) const;  
};

