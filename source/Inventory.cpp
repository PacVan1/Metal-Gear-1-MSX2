#include "precomp.h"
#include "Inventory.h"

#include "Game.h"
#include "Projectile.h" 

Inventory::Inventory() :
	mSelectedWeapon(nullptr),
	mSelectedEquipment(nullptr)
{
	Item::spriteSheet = SpriteSheet("assets/spritesheets/equipment/equipment.sheet");
	Item::spriteSheet2 = SpriteSheet("assets/spritesheets/weapons/weapons.sheet");
	Item::palette = ColorPalette8("assets/color_palettes/item.cpalette");
	Item::spriteSheet.palette = &Item::palette;
	Item::spriteSheet2.palette = &Item::palette;

	mItems[ITEM_TYPES_HANDGUN]		= new Handgun();
	mItems[ITEM_TYPES_LAND_MINE]	= new Landmine();
	mItems[ITEM_TYPES_BINOCULARS]	= new Binoculars();
	mItems[ITEM_TYPES_GAS_MASK]		= new GasMask();  
	mItems[ITEM_TYPES_CARD1]		= new Card(1);
	mItems[ITEM_TYPES_CARD2]		= new Card(2);
	mItems[ITEM_TYPES_CARD3]		= new Card(3);
	mItems[ITEM_TYPES_CARD4]		= new Card(4);
	mItems[ITEM_TYPES_CARD5]		= new Card(5);
	mItems[ITEM_TYPES_CARD6]		= new Card(6);
	mItems[ITEM_TYPES_CARD7]		= new Card(7);
	mItems[ITEM_TYPES_CARD8]		= new Card(8);

	Unlock(ITEM_TYPES_LAND_MINE);
	mItems[ITEM_TYPES_LAND_MINE]->Stack(99);

#if EVERYTHING_UNLOCKED
	// default selected items: 
	for (int i = 0; i < ITEM_TYPES_COUNT; i++) { Unlock(i); mItems[i]->Stack(1); } 

	mSelectedWeapon		= mItems[ITEM_TYPES_HANDGUN];
	mSelectedEquipment	= mItems[ITEM_TYPES_CARD7];
#endif
}

Inventory::~Inventory()
{
	for (int i = 0; i < ITEM_TYPES_COUNT; i++)
	{
		delete mItems[i];
	}
}

void Inventory::PickUp(ItemObject& item) 
{
	if (item.mType == ITEM_TYPES_AMMO)
	{
		if (mSelectedWeapon)
		{
			mSelectedWeapon->Stack(mItems[item.mType]->count);
		}
	}
	else if (!mUnlocked[item.mType])
	{
		Unlock(item.mType);  
	}
	else // unlocked
	{
		mItems[item.mType]->Stack(1);
	}
	item.mPickedUp = true;

	mItems[item.mType]->PickUp();

	Game::SetTextPopup(item.mPosI + int2(10, -10), mItems[item.mType]->name);
}

void Inventory::Unlock(int itemType)
{
	mUnlocked[itemType] = true;
	if (itemType < ITEM_TYPES_WEAPON_COUNT)
	{
		mSelectedWeapon = mItems[itemType];
		mUnlockedWeapons[mUnlockedWeaponsCount++] = mItems[itemType]; 
	}
	else
	{
		mSelectedEquipment = mItems[itemType];
		mUnlockedEquipment[mUnlockedEquipmentCount++] = mItems[itemType];
	}
}

void Inventory::UseWeapon()
{
	if (mSelectedWeapon) mSelectedWeapon->Use();
}

void Inventory::UseEquipment()
{
	if (mSelectedEquipment) mSelectedEquipment->Use();
}

Handgun::Handgun() :
	mDir(0.0f)
{
	mFrameIdx = 0;
	stackable = true;
	count = 90;
	mSpriteSheet = &Item::spriteSheet2; 
	strcpy(name, "Handgun");
}

void Handgun::PickUp()
{
	Game::sPlayer.SetBehavior(Game::sPlayer.GetBehavior());
}

void Handgun::Use()
{
	float2 const from = float2(0.0f, -15.0f) + Game::sPlayer.GetPosition() + mOffset * cardinalToFloat2(Game::sPlayer.GetFacing());
	Game::sProjectiles.Launch(from, cardinalToFloat2(Game::sPlayer.GetFacing()) * mSpeed, mDamage, mCollisionGroup);
}

Landmine::Landmine() 
{
	mFrameIdx = 4;
	strcpy(name, "Landmine");
	mSpriteSheet = &Item::spriteSheet2;
}

void Landmine::Use()
{
	int const idx = Game::sLandmines.WakeObject();
	if (idx != WAKE_TYPES_FULL)
	{
		LandmineObject& landmine = Game::sLandmines[idx]; 
		landmine.Reset();
		landmine.SetPosition(Game::sPlayer.GetPosition());
	}
}

Binoculars::Binoculars() 
{
	mFrameIdx = 3;
	strcpy(name, "Scope");
	mSpriteSheet = &Item::spriteSheet;
}

void Binoculars::Use()
{
	Game::EnterPeakState(static_cast<cardinals>(Game::sPlayer.GetFacing()));
}

GasMask::GasMask()
{
	mFrameIdx = 5;
	stackable = false;
	strcpy(name, "G.Mask");
	mSpriteSheet = &Item::spriteSheet;
}

Card::Card(int level) :
	level(level)
{
	mFrameIdx = 4;
	stackable = false;

	strcpy(name, "Card");
	char strLevel[3]; sprintf(strLevel, "%d", level);
	strcat(name, strLevel);
	mSpriteSheet = &Item::spriteSheet;
}

void Card::Use()
{
	for (int i = 0; i < Game::GetScene().mPassageCount; i++)
	{
		Passage& passage = *Game::GetScene().mPassages[i];
		if (!passage.sharedData->mUnlocked && AABB::Detect(passage.mBbox, Game::sPlayer.GetBbox()))
		{
			passage.sharedData->mUnlocked = true;
			if (passage.props.dataUnlocked.tile)
			{
				Game::GetTilemap().InsertMetaTile(passage.mTileCoord, passage.props.dataUnlocked);
			}
		}
	}
}
