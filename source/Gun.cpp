#include "precomp.h"
#include "Gun.h"

#include "Projectile.h"
#include "Inventory.h"
#include "Game.h" 

Gun::Gun()
{
	frameIdx = 0;
	stackable = true;
	count = 90; 
	strcpy(name, "Handgun");
}

void Gun::Use()
{
	Projectile::Launch(Game::player.GetPosition(), CardinalToFloat2(Game::player.facing) * speed, damage, AABB::ENEMY);
}
