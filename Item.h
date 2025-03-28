#pragma once

#include "GameObject.h"
#include "SpriteSheet.h" 

class Item
{
public:
	inline static SpriteSheet	spriteSheet; 
	inline static ColorPalette8 palette; 

public:
	uint	frameIdx;
	uint	count; 
	char*	name;
	bool	stackable; 

public:
	Item(); 
	Item(Item const& item);
	virtual void Equip() {}
	virtual void Use() {}
	void Stack(uint const amount) { if (stackable) count += amount; } 
};

class ItemObject : public GameObject
{
public:
	int		type;
	bool	pickedUp;

public:
	ItemObject(int type);
	void Render(Surface8* screen) const;
};

