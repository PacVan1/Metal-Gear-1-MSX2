#include "precomp.h"
#include "Item.h"

Item::Item() :
	name(new char[32])
{}

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
