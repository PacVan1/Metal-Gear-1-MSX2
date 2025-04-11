#include "precomp.h"
#include "Projectile.h"

#include "Game.h"
#include "Player.h"
#include "Soldiers.h"
#include "TileMap.h"

Projectile::Projectile() :
	mBbox(&mPosF, 3, -1.0f), 
	mSpeed(0.0f)
{}

void Projectile::Update(float const dt)
{
	mPosF += mSpeed * dt; 
	mBbox.Update(); 
}

void Projectile::Render(Surface8* screen)
{
	int2 const pos = static_cast<int2>(mPosF);  
	screen->Bar(pos.x - 1, pos.y - 1, pos.x + 1, pos.y + 1, 181);
	mBbox.Render(screen); 
}

void Projectile::SetPosition(float2 const position)
{
	mPosF = position;
	mBbox.Update(); 
}

void Projectile::Launch(float2 const position, float2 const speed, int const damage, int const collisionGroup)
{
	SetPosition(position); 
	mSpeed			= speed;
	mDamage			= damage;
	mBbox.mGroup	= collisionGroup;
}

Projectiles::Projectiles() :
	mPool(PROJECTILE_COUNT)
{}

void Projectiles::Update(float const dt)
{
	for (int i = 0; i < mPool.SIZE; i++)
	{
		if (mPool.IsActive(i))
		{
			mPool[i].Update(dt);
		}
	}

	for (int i = 0; i < mPool.SIZE; i++)
	{
		if (mPool.IsActive(i))
		{
			Projectile& projectile = mPool[i];
			int tileState = Game::GetTilemap().GetTileState(PixelToTile(static_cast<int2>(projectile.GetPosition())));
			if (tileState == TilePalette::Tile::SOLID ||
				!AABB::Detect(projectile.GetBbox(), NATIVE_SCREEN_WIDTH - 2.0f, 0.0f, 0.0f, NATIVE_SCREEN_HEIGHT - 22.0f))
			{
				mPool.ReturnObject(i);
			}
		}
	}

	for (int i = 0; i < mPool.SIZE; i++)
	{
		if (mPool.IsActive(i))
		{
			Projectile& projectile = mPool[i];
			//if (AABB::DetectGroup(Game::sPlayer.GetBbox(), projectile.GetBbox()))
			//{
			//	mPool.ReturnObject(i);
			//	Game::sPlayer.Damage(projectile.GetDamage());
			//}
			if (Game::sPlayer.DetectGrouped(projectile.GetBbox()))
			{
				mPool.ReturnObject(i);
				Game::sPlayer.Damage(projectile.GetDamage());
			}
		}
	}
	
	for (int i = 0; i < mPool.SIZE; i++)
	{
		if (mPool.IsActive(i))
		{
			Projectile& projectile = mPool[i];
			for (int j = 0; j < Game::sSoldiers.mPool.SIZE; j++)
			{
				if (Game::sSoldiers.mPool.IsActive(j) && !Game::sSoldiers.mPool[j].mDestroyed)
				{
					//if (AABB::DetectGroup(Game::sSoldiers.mPool[j].GetBbox(), projectile.GetBbox()))
					//{
					//	mPool.ReturnObject(i);
					//	Game::sSoldiers.mPool[j].Damage(projectile.GetDamage());
					//}
					if (Game::sSoldiers.mPool[j].DetectGrouped(projectile.GetBbox()))
					{
						mPool.ReturnObject(i);
						Game::sSoldiers.mPool[j].Damage(projectile.GetDamage());
					}
				}
			}
		}
	}
}

void Projectiles::Render(Surface8* screen) 
{
	for (int i = 0; i < mPool.SIZE; i++)
	{
		if (mPool.IsActive(i))
		{
			mPool[i].Render(screen);
		}
	}
}

void Projectiles::Launch(float2 const position, float2 const speed, int const damage, int const collisionGroup)
{
	// TODO check if there are projectiles in the pool before waking one:
	mPool[mPool.WakeObject()].Launch(position, speed, damage, collisionGroup); 
}
