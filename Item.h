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
	char	strCount[3];
	char	name[32]; 
	bool	stackable; 

public:
	Item(); 
	Item(Item const& item);
	virtual void Equip() {}
	virtual void Use() {}
	void Stack(uint const amount) { if (stackable) { count += amount; sprintf(strCount, "%d", count); } }
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

class Card : public Item
{
public:
	int level;

public:
	Card(int level);
	void Use() override; 
};

