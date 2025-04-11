#include "precomp.h"
#include "Item.h"

#include "Game.h" 

Item::Item() :
	count(0),
	mFrameIdx(0),
	mSpriteSheet(nullptr)
{
	strcpy(name, "-");
	sprintf(strCount, "%d", 0);
}

Item::Item(Actor const* owner) :
	mOwner(owner), 
	count(0),
	mFrameIdx(0),
	mSpriteSheet(nullptr)
{
	strcpy(name, "-");
	sprintf(strCount, "%d", 0);
}

Item::Item(Item const& item) :
	count(0),
	mFrameIdx(0),
	mSpriteSheet(nullptr)
{
	mFrameIdx = item.mFrameIdx;
	strcpy(name, item.name);
	sprintf(strCount, "%d", 0);
}

void Item::Render(Surface8* screen, int const x, int const y) const
{
	if (mSpriteSheet) mSpriteSheet->RenderFrame(screen, x, y, mFrameIdx);
}

ItemObject::ItemObject() :
	mPosF(0.0f),
	mPosI(0),
	mType(ITEM_TYPES_NONE), 
	mPickedUp(false),
	mBbox(&mPosF)
{
	mBbox.Update(); 
}

ItemObject::ItemObject(ItemObject const& other) :
	mPosF(other.mPosF),
	mPosI(other.mPosF), 
	mType(other.mType), 
	mPickedUp(other.mPickedUp),
	mBbox(&mPosF) 
{
	mBbox.Update();
}

ItemObject::ItemObject(int const type) :
	mPosF(0.0f),
	mPosI(0),
	mType(type), 
	mPickedUp(false),
	mBbox(&mPosF)
{
	SetType(type); 
	mBbox.Update(); 
}

void ItemObject::Render(Surface8* screen) const
{
	Game::sInventory.mItems[mType]->Render(screen, mPosI.x + mBbox.GetOffset().x, mPosI.y + mBbox.GetOffset().y);
	mBbox.Render(screen); 
}

void ItemObject::SetPosition(int2 const position)
{
	mPosF = static_cast<float2>(position);
	mPosI = position;
	mBbox.Update(); 
}

void ItemObject::SetType(int const type)
{
	mType = type;
	mPickedUp = false;
	mBbox.Resize({ Game::sInventory.mItems[mType]->mSpriteSheet->mFrameWidth, 16 }, { -Game::sInventory.mItems[mType]->mSpriteSheet->mFrameWidth / 2, -8 });
}
