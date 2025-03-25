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
	static float constexpr SPAWN_TIME	= 3.0f; 

public:
	inline static ObjectPool<Soldier>	pool = ObjectPool<Soldier>(SOLDIER_COUNT);
	inline static int					alertLevel = OFF;
	inline static Timer					spawnTimer; 

public:
	static void Update(float const dt);
	static void Render(Surface8* screen);

	static void SetAlertLevel(int alertLevel);  
	static void Damage(int idx, int damage);  
	static int2 FindSpawnTile();
	static void SpawnReinforcement();
};

