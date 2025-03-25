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
	if (spawnTimer.elapsed() > 3.0f)
	{
		spawnTimer.reset();
		SpawnReinforcement();
	}
}

void Soldiers::Render(Surface8* screen) const
{
	for (int i = 0; i < pool.SIZE; i++)
	{
		if (pool.active[i] && !pool[i].destroyed)
		{
			pool[i].Render(screen);
		}
	}
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