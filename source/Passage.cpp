#include "precomp.h"
#include "Passage.h"

#include "Game.h" 

Passage::Passage(PassageProps const& props) :
	GameObject(TileToPixel({ props.dataLocked.columns, props.dataLocked.rows }), 0), 
	props(props) 
{}

void Passage::SetPosition(float2 const position)
{
	GameObject::SetPosition(position);
	spawnPosition = GetPositionInt() + props.spawnPosition;  
}

void Passage::Render(Surface8* screen) const
{
	bbox.Render(screen);  
}

void Passage::TryEnter() const
{
	if (!sharedData->unlocked) return; 

	if (AABB::Detect(Game::player.bboxTile, bbox))
	{
		Game::world.SwitchScene(other->scene);
		Game::player.SetPosition(other->spawnPosition); 
	}
}