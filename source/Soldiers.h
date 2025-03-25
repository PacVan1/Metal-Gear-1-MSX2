#pragma once

#include "ObjectPool.h"
#include "Soldier.h" 

class Soldiers
{
public:
	enum alertLevels
	{
		OFF, LOW, HIGH
	};

public:
	static uint constexpr SOLDIER_COUNT = 6;  

public:
	ObjectPool<Soldier> pool; 
	Timer spawnTimer; 

public:
	Soldiers() = default; 
	void Update(float const dt);
	void Render(Surface8* screen) const;

	int2 FindSpawnTile();
	void SpawnReinforcement();
};

