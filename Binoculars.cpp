#include "precomp.h"
#include "Binoculars.h"

#include "Game.h" 

Binoculars::Binoculars()
{
	strcpy(name, "Binoculars"); 
}

void Binoculars::Use()
{
	Game::world.SwitchScene(Game::player.facing);
}
