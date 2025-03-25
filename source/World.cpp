#include "precomp.h"
#include "World.h"

#include "AABB.h" 
#include "Soldier.h"
#include "Game.h"

void World::InitWorld()
{
	currentSceneCoord	= int2(0); 
	currentSceneIdx		= 0;
	currentScene = &scenes[currentSceneIdx];

	for (int i = 0; i < currentScene->soldierCount; i++)
	{
		if (Game::soldiers.pool.activeCount < Soldiers::SOLDIER_COUNT)
		{
			Soldier& soldier = Game::soldiers.pool[Game::soldiers.pool.WakeObject()]; 
			soldier.sequencer.SetSequence(currentScene->paths[i]);
			soldier.SetState(Soldier::IDLE);
			soldier.ResetHealth();
		}
	}

	AABB::currentTilemap = currentScene->tilemap;
}

void World::SwitchScene(cardinals cardinal)
{
	int2 coord = currentSceneCoord + CardinalToInt2(cardinal);
	if (AABB::Detect(coord, columns, 0, 0, rows))
	{
		int sceneIdx = coord.x + coord.y * columns; 
		if (sceneIdx != currentSceneIdx) 
		{
			currentSceneIdx		= sceneIdx;
			currentSceneCoord	= coord; 

			Game::soldiers.pool.ReturnAll(); 

			currentScene		= &scenes[currentSceneIdx];

			for (int i = 0; i < currentScene->soldierCount; i++)
			{
				if (Game::soldiers.pool.activeCount < Soldiers::SOLDIER_COUNT)
				{
					Soldier& soldier = Game::soldiers.pool[Game::soldiers.pool.WakeObject()];
					soldier.sequencer.SetSequence(currentScene->paths[i]);
					soldier.SetState(Soldier::IDLE);
					soldier.ResetHealth(); 
				}
			}

			AABB::currentTilemap = currentScene->tilemap;
			//Soldier::SetAlertState(Soldier::alertStates::OFF);
 		}
	}
}
