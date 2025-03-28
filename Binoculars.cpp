#include "precomp.h"
#include "Binoculars.h"

#include "Game.h" 

void Binoculars::Use()
{
	Game::world.SwitchScene(Game::player.facing);
}
