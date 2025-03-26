#pragma once

#include "GameObject.h"
#include "ObjectPool.h" 

class Player;

class Projectile : public GameObject
{
public:
	static Player*			player;  
	static ObjectPool<Projectile>	projectiles; 

public:
	float2 speed;
	uint damage;

public:
	static void UpdatePool(float const dt);
	static void RenderPool(Surface8* screen);
	static void Launch(float2 pos, float2 speed, uint damage, int group); 

public:
	Projectile(); 
	void Update(float const dt);
	void Render(Surface8* screen);  
};
