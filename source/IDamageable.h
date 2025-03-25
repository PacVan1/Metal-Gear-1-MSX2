#pragma once

class IDamageable
{
public:
	int health;  
	int maxHealth;
	int destroyed = false;

public:
	virtual void Damage(int damage) = 0; 
	virtual void Destroy() { destroyed = true; }
	void ResetHealth() { health = maxHealth; destroyed = false; }
};

