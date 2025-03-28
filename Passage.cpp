#include "precomp.h"
#include "Passage.h"

#include "Game.h" 

Passage::Passage() :
	GameObject(10, 0)
{}

void Passage::TryEnter() const
{
	if (!sharedData->unlocked) return; 

	if (AABB::Detect(Game::player.bboxTile, bbox))
	{
		Game::world.SwitchScene(other->scene);
		Game::player.SetPosition(other->spawnPosition); 
	}
}
