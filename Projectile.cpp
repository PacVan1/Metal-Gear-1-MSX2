#include "precomp.h"
#include "Projectile.h"

#include "Player.h"
#include "TileMap.h"

ObjectPool<Projectile> Projectile::projectiles = ObjectPool<Projectile>(100);  
Player* Projectile::player = nullptr; 

void Projectile::UpdatePool(float const dt)
{
	for (int i = 0; i < projectiles.SIZE; i++)
	{
		if (projectiles.active[i])
		{
			projectiles[i].Update(dt);

			if (!AABB::Detect(projectiles[i].bbox, NATIVE_SCREEN_WIDTH - 2.0f, 0.0f, 0.0f, NATIVE_SCREEN_HEIGHT - 22.0f))
			{
				projectiles.active[i] = false;
				continue; // I am sorry Abhishek
			}
			// longest ugliest line ever, I know:
			int tileState = AABB::currentTilemap->GetTileState(PixelToTile(projectiles[i].bbox.iPos));
			if (tileState == TilePalette::Tile::SOLID)
			{
				projectiles.active[i] = false;
				return; 
			}

			if (AABB::DetectGroup(player->bbox, projectiles[i].bbox))
			{
				projectiles.active[i] = false;
				player->Damage(1);
			}
		}
	}
}

void Projectile::RenderPool(Surface8* screen)
{
	for (int i = 0; i < projectiles.SIZE; i++)
	{
		if (projectiles.active[i])
		{
			projectiles[i].Render(screen); 
		}
	}
}

void Projectile::Launch(float2 pos, float2 speed, int group)
{
	uint idx = projectiles.WakeObject();
	projectiles[idx].SetPosition(pos);
	projectiles[idx].speed = speed;
	projectiles[idx].bbox.group = group; 
}

Projectile::Projectile() :
	GameObject(3, 1),
	speed(0.0f)
{}

void Projectile::Update(float const dt)
{
	SetPosition(bbox.fPos + speed * dt);
}

void Projectile::Render(Surface8* screen)
{
	screen->Bar(bbox.iPos.x - 1, bbox.iPos.y - 1, bbox.iPos.x + 1, bbox.iPos.y + 1, 40);
	bbox.Render(screen);  
}
