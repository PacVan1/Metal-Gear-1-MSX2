#include "precomp.h"
#include "Item.h"

#include "Game.h" 

Item::Item() :
	name(new char[32])
{
	strcpy(name, "No name");
}

Item::Item(Item const& item) :
	name(new char[32]) // max name length
{
	frameIdx = item.frameIdx;
	strcpy(name, item.name);  
}

ItemObject::ItemObject(int type) :
	GameObject(10, -5),
	type(type),
	pickedUp(false) 
{}

void ItemObject::Render(Surface8 * screen) const
{
	//sprite.Render(screen, bbox.iPos.x, bbox.iPos.y);
	bbox.Render(screen); 
}

Card::Card(int level) :
	level(level)
{
	stackable = false;

	strcpy(name, "Card");
	char strLevel[3]; sprintf(strLevel, "%d", level); 
	strcat(name, strLevel);  
}

void Card::Use()
{
	if (!Game::passage1->sharedData->unlocked && AABB::Detect(Game::passage1->bbox, Game::player.bbox))
	{
		Game::passage1->sharedData->unlocked = true;
		Game::passage1->scene->tilemap->InsertMetaTile({ 12, 12 }, Game::passage1->props.dataUnlocked);
		Game::passage1->other->scene->tilemap->InsertMetaTile({ 20, 12 }, Game::passage1->other->props.dataUnlocked);
	}

	if (!Game::passage2->sharedData->unlocked && AABB::Detect(Game::passage2->bbox, Game::player.bbox))
	{
		Game::passage2->sharedData->unlocked = true;
		Game::passage2->scene->tilemap->InsertMetaTile({ 20, 12 }, Game::passage2->props.dataUnlocked);
		Game::passage2->other->scene->tilemap->InsertMetaTile({ 12, 12 }, Game::passage2->other->props.dataUnlocked);
	}
}
