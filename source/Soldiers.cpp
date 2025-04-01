#include "precomp.h"
#include "Soldiers.h"

#include "Tilemap.h" 

void Soldiers::Update(float const dt)
{
	if (pool.GetActiveCount() == 0) return;

	// update soldiers: 
	for (int i = 0; i < pool.SIZE; i++)
	{
		if (pool.IsActive(i) && !pool[i].destroyed)
		{
			pool[i].Update(dt);
		}
	}

	// call new reinforcements:
	if (alertLevel == HIGH)
	{
		if (spawnTimer.elapsed() > SPAWN_TIME) 
		{
			spawnTimer.reset();
			SpawnReinforcement();
		}
	}
}

void Soldiers::Render(Surface8* screen)
{
	if (pool.GetActiveCount() == 0) return;

	for (int i = 0; i < pool.SIZE; i++)
	{
		if (pool.IsActive(i) && !pool[i].destroyed)
		{
			pool[i].Render(screen);
		}
	}
}

#include "Game.h"

void Soldiers::SetAlertLevel(int level)
{
	if (alertLevel == level) return; 

	alertLevel = level; 
	switch (alertLevel)
	{
	case OFF:	Game::SetTheme(&Game::mainTheme); break;
	case SPOTTED: break; 
	case LOW:
	{
		Game::SetTheme(&Game::alertTheme);

		for (int i = 0; i < pool.SIZE; i++)
		{
			if (pool.IsActive(i) && !pool[i].destroyed)
			{
				pool[i].Alert(); 
			}
		}

		break;
	}
	case HIGH:
	{
		Game::SetTheme(&Game::alertTheme);
		spawnTimer.reset();

		for (int i = 0; i < pool.SIZE; i++)
		{
			if (pool.IsActive(i) && !pool[i].destroyed)
			{
				pool[i].Alert();
			}
		}

		break;
	}
	}
}

void Soldiers::Damage(int idx, int damage)
{
	if (pool.GetActiveCount() == 0) return;

	// damage:
	pool[idx].Damage(damage);

	if (pool.GetActiveCount() == 0) return;

	// check if everybody is dead:
	for (int i = 0; i < SOLDIER_COUNT; i++)
	{
		if (pool.IsActive(i) && !pool[i].destroyed)
		{
			return; 
		}
	}

	SetAlertLevel(OFF);
}

int2 Soldiers::FindSpawnTile()
{
	for (int i = 0; i < cardinals::COUNT; i++)
	{
		int2 coord = FindSpawnTileOnSide(i);
		if (coord != int2(-1, -1)) return coord; 
	}
	return int2(-1, -1); 
}

void Soldiers::SpawnReinforcement()
{
	if (pool.GetActiveCount() >= SOLDIER_COUNT) return;

	int2 coord = FindSpawnTile();
	if (coord == int2(-1, -1)) return; 

	Soldier& soldier = pool[pool.WakeObject()]; 
	soldier.SetPosition(TileToPixel(coord));  
	soldier.Alert(); 
	soldier.ResetHealth(); 
}

int2 Soldiers::FindSpawnTileOnSide(int cardinal)
{
	// TODO devise a more sophisticated approach: 
	// give it ten chances:
	int2 side = CardinalToInt2(cardinal);  
	for (int i = 0; i < 10; i++) 
	{
		int2 coord = int2(	side.x * (RandomUInt() % Tilemap::COLUMNS),
							side.y * (RandomUInt() % Tilemap::ROWS));
		float2 pos = TileToPixel(coord);
		TileArea area = SubPixelToTileArea(pos - pool[0].bboxTile.HALF_SIZE, pos + pool[0].bboxTile.HALF_SIZE);
		if (AABB::DetectTilemap(area)) return coord; 
	}

	return int2(-1, -1); 
}

void Soldiers::SpawnReinforcementOnSide(int cardinal)
{
	if (pool.GetActiveCount() >= SOLDIER_COUNT) return;

	int2 coord = FindSpawnTileOnSide(cardinal);
	if (coord == int2(-1, -1)) return;

	Soldier& soldier = pool[pool.WakeObject()];
	soldier.SetPosition(TileToPixel(coord));
	soldier.Alert();
	soldier.ResetHealth();
}
