#include "precomp.h"
#include "Inventory.h"

#include "Game.h"
#include "Projectile.h" 

Inventory::Inventory() :
	selectedWeapon(nullptr),
	selectedEquipment(nullptr)
{
	items[HANDGUN]		= new Handgun();
	items[LAND_MINE]	= new Landmine();
	items[BINOCULARS]	= new Binoculars();
	items[GASMASK]		= new GasMask(); 
	items[CARD1]		= new Card(1);
	items[CARD2]		= new Card(2);
	items[CARD3]		= new Card(3);
	items[CARD4]		= new Card(4);
	items[CARD5]		= new Card(5);
	items[CARD6]		= new Card(6);
	items[CARD7]		= new Card(7);
	items[CARD8]		= new Card(8);

#if EVERYTHING_UNLOCKED
	// default selected items: 
	for (int i = 0; i < COUNT; i++) { Unlock(i); items[i]->Stack(1); }

	selectedWeapon		= items[HANDGUN]; 
	selectedEquipment	= items[BINOCULARS];
#endif
}

Inventory::~Inventory()
{
	for (int i = 0; i < COUNT; i++)
	{
		delete items[i];
	}
}

void Inventory::PickUp(ItemObject& item) 
{
	if (item.type == AMMO)
	{
		if (selectedWeapon)
		{
			selectedWeapon->Stack(items[item.type]->count);
		}
	}
	else if (!unlocked[item.type])
	{
		Unlock(item.type);  
	}
	else // unlocked
	{
		items[item.type]->Stack(1);
	}
	item.pickedUp = true;

	Game::SetTextPopup(item.GetPositionInt() + int2(10, -10), items[item.type]->name);
}

void Inventory::Unlock(int itemType)
{
	unlocked[itemType] = true;
	if (itemType < WEAPON_COUNT)
	{
		selectedWeapon = items[itemType];
		unlockedWeapons[unlockedWeaponsCount++] = items[itemType]; 
	}
	else
	{
		selectedEquipment = items[itemType];
		unlockedEquipment[unlockedEquipmentCount++] = items[itemType];
	}
}

Handgun::Handgun()
{
	frameIdx = 0;
	stackable = true;
	count = 90;
	strcpy(name, "Handgun");
}

void Handgun::Use()
{
	Projectile::Launch(Game::player.GetPosition(), CardinalToFloat2(Game::player.facing) * speed, damage, AABB::ENEMY);
}

Landmine::Landmine()
{
	strcpy(name, "Landmine");
}

Binoculars::Binoculars()
{
	strcpy(name, "Binoculars");
}

void Binoculars::Use()
{
	Game::world.SwitchScene(Game::player.facing);
}

GasMask::GasMask()
{
	stackable = false;
	strcpy(name, "GasMask");  
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
	if (level >= Game::passage1->sharedData->accessLevel)
	{
		if (!Game::passage1->sharedData->unlocked && AABB::Detect(Game::passage1->bbox, Game::player.bbox))
		{
			Game::passage1->sharedData->unlocked = true;
			Game::passage1->scene->tilemap->InsertMetaTile({ 12, 12 }, Game::passage1->props.dataUnlocked);
			Game::passage1->other->scene->tilemap->InsertMetaTile({ 20, 12 }, Game::passage1->other->props.dataUnlocked);
		}
	}

	if (level >= Game::passage2->sharedData->accessLevel)
	{
		if (!Game::passage2->sharedData->unlocked && AABB::Detect(Game::passage2->bbox, Game::player.bbox))
		{
			Game::passage2->sharedData->unlocked = true;
			Game::passage2->scene->tilemap->InsertMetaTile({ 20, 12 }, Game::passage2->props.dataUnlocked);
			Game::passage2->other->scene->tilemap->InsertMetaTile({ 12, 12 }, Game::passage2->other->props.dataUnlocked);
		}
	}
}
