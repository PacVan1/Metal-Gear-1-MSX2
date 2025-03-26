#include "precomp.h"
#include "Gun.h"

#include "Projectile.h"

void Gun::Shoot(float2 from, float2 to) const
{
	Projectile::Launch(from, to * speed, damage, AABB::ENEMY); 
}
