#include "precomp.h"
#include "Item.h"

#include "Game.h" 

Item::Item() :
	count(0)
{
	strcpy(name, "-");
	sprintf(strCount, "%d", 0);
}

Item::Item(Item const& item) :
	count(0)
{
	frameIdx = item.frameIdx;
	strcpy(name, item.name);
	sprintf(strCount, "%d", 0);
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
