#pragma once

class IDamageable
{
public:
	int mHealth;  
	int mMaxHealth;
	int mDestroyed;

public:
	virtual void Damage(int damage) = 0; 
	virtual void Destroy() { mDestroyed = true; }
	void ResetHealth() { mHealth = mMaxHealth; mDestroyed = false; }
};

