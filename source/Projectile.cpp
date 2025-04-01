#include "precomp.h"
#include "Projectile.h"

#include "Player.h"
#include "Soldiers.h"
#include "TileMap.h"

ObjectPool<Projectile> Projectile::projectiles = ObjectPool<Projectile>(100);  
Player* Projectile::player = nullptr; 

void Projectile::UpdatePool(float const dt)
{
	for (int i = 0; i < projectiles.SIZE; i++)
	{
		if (projectiles.IsActive(i))
		{
			projectiles[i].Update(dt);

			if (!AABB::Detect(projectiles[i].bbox, NATIVE_SCREEN_WIDTH - 2.0f, 0.0f, 0.0f, NATIVE_SCREEN_HEIGHT - 22.0f))
			{
				projectiles.ReturnObject(i); 
				continue; // I am sorry Abhishek
			}
			// longest ugliest line ever, I know:
			int tileState = AABB::currentTilemap->GetTileState(PixelToTile(projectiles[i].GetPositionInt()));
			if (tileState == TilePalette::Tile::SOLID)
			{
				projectiles.ReturnObject(i);
				return; 
			}

			if (AABB::DetectGroup(player->bbox, projectiles[i].bbox))
			{
				projectiles.ReturnObject(i);
				player->Damage(projectiles[i].damage); 
			}

			for (int i = 0; i < Soldiers::pool.SIZE; i++)
			{
				if (Soldiers::pool.IsActive(i) && !Soldiers::pool[i].destroyed)
				{
					if (AABB::DetectGroup(Soldiers::pool[i].bbox, projectiles[i].bbox)) 
					{
						projectiles.ReturnObject(i);
						Soldiers::pool[i].Damage(projectiles[i].damage);
					}
				}
			}
		}
	}
}

void Projectile::RenderPool(Surface8* screen)
{
	for (int i = 0; i < projectiles.SIZE; i++)
	{
		if (projectiles.IsActive(i))
		{
			projectiles[i].Render(screen); 
		}
	}
}

void Projectile::Launch(float2 pos, float2 speed, uint damage, int group)
{
	uint idx = projectiles.WakeObject();
	projectiles[idx].SetPosition(pos);
	projectiles[idx].speed = speed;
	projectiles[idx].damage = damage;
	projectiles[idx].bbox.group = group; 
}

Projectile::Projectile() :
	GameObject(3, 1),
	speed(0.0f)
{}

void Projectile::Update(float const dt)
{
	SetPosition(GetPosition() + speed * dt);
}

void Projectile::Render(Surface8* screen)
{
	screen->Bar(GetPositionInt().x - 1, GetPositionInt().y - 1, GetPositionInt().x + 1, GetPositionInt().y + 1, 40);
	bbox.Render(screen);  
}
