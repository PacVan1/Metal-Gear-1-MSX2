#pragma once

#include "AABB.h" 
#include "SpriteSheet.h" 

class Actor;

class Item
{
public:
	inline static SpriteSheet	spriteSheet;
	inline static SpriteSheet	spriteSheet2;
	inline static ColorPalette8 palette;

public:
	Actor const*		mOwner;
	uint				mFrameIdx;
	uint				count;
	char				strCount[3];
	char				name[32];
	bool				stackable;
	SpriteSheet const*	mSpriteSheet;

public:
	Item();
	Item(Actor const* owner);
	Item(Item const& item);
	virtual void	PickUp() {}
	virtual void	Equip() {}
	virtual void	Use() {}
	void			Stack(uint const amount) { if (stackable) { count += amount; sprintf(strCount, "%d", count); } }
	void			Render(Surface8* screen, int const x, int const y) const;
};

class ItemObject
{
public:
	float2	mPosF;
	int2	mPosI; 
	int		mType;
	bool	mPickedUp;
	AABB	mBbox;

public:
			ItemObject();
			ItemObject(ItemObject const& other); 
			ItemObject(int const type);
	void	Render(Surface8* screen) const;

	void	SetPosition(int2 const position);
	void	SetType(int const type);  

	inline AABB const& GetBbox() const { return mBbox; }
};



