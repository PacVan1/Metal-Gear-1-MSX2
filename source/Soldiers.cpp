#include "precomp.h"
#include "Soldiers.h"

#include "Tilemap.h" 

void Soldiers::Update(float const dt)
{
	// update soldiers: 
	for (int i = 0; i < pool.SIZE; i++)
	{
		if (pool.active[i] && !pool[i].destroyed)
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
	for (int i = 0; i < pool.SIZE; i++)
	{
		if (pool.active[i] && !pool[i].destroyed)
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
	case LOW:	Game::SetTheme(&Game::alertTheme); break;
	case HIGH:	Game::SetTheme(&Game::alertTheme); spawnTimer.reset(); break;
	}
}

void Soldiers::Damage(int idx, int damage)
{
	// damage:
	pool[idx].Damage(damage);

	// check if everybody is dead:
	for (int i = 0; i < SOLDIER_COUNT; i++)
	{
		if (pool.active[i] && !pool[i].destroyed)
		{
			return; 
		}
	}

	SetAlertLevel(OFF);
}

int2 Soldiers::FindSpawnTile()
{
	return int2(RandomUInt() * (Tilemap::COLUMNS - 1), RandomUInt() * (Tilemap::ROWS - 1));
}

void Soldiers::SpawnReinforcement()
{
	if (pool.activeCount >= SOLDIER_COUNT) return;

	Soldier& soldier = pool[pool.WakeObject()]; 
	soldier.SetPosition(/*TileToPixel(FindSpawnTile())*/ 100.0f);  
	soldier.SetState(Soldier::PERSUE);
	soldier.DecideCardinal();  
	soldier.SetAnimation();
	soldier.ResetHealth();
}