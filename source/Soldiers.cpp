#include "precomp.h"
#include "Soldiers.h"

#include "SoldierTypes.h" 
#include "Tilemap.h" 
#include "Game.h" 

Soldiers::Soldiers() :
	mPool(SOLDIER_COUNT),
	mAlertLevel(ALERT_LEVELS_OFF),
	mSpawnAlarm(SOLDIER_SPAWN_TIME)
{}

void Soldiers::Update(float const dt)
{
	for (int i = 0; i < mPool.GetActiveCount(); i++) 
	{
		Soldier& soldier = mPool.GetActive(i);
		if (!soldier.mDestroyed) soldier.Update(dt);  
	}

	if (mAlertLevel == ALERT_LEVELS_HIGH) 
	{
		if (mSpawnAlarm.Elapsed()) 
		{
			mSpawnAlarm.Reset(); 
			SpawnReinforcement();
		}
	}
}

void Soldiers::Render(Surface8* screen) const
{
	if (mPool.GetActiveCount() == 0) return; 

	for (int i = 0; i < mPool.GetActiveCount(); i++)
	{
		Soldier const& soldier = mPool.GetActive(i);
		if (!soldier.mDestroyed) soldier.Render(screen);  
	}

	if (mAlertLevel == ALERT_LEVELS_SPOTTED)
	{
		mAlertPopup.Render(screen); 
	}
}

void Soldiers::SetType(int const typeIdx)
{
	mType = &Game::sSoldierTypes.mTypeData[typeIdx];
	Game::sSoldierTypes.mSpriteSheet.palette = &mType->mPalette;
}

bool Soldiers::AreDead() const
{
	for (int i = 0; i < mPool.GetActiveCount(); i++)
	{
		Soldier const& soldier = mPool.GetActive(i);
		if (!soldier.mDestroyed) return false; 
	}
	return true;
}

void Soldiers::SetAlertLevel(int const alertLevel) 
{
	if (mAlertLevel == alertLevel) return;

	mAlertLevel = alertLevel;
	switch (mAlertLevel)
	{
	case ALERT_LEVELS_OFF:		Game::SetMusic(MUSIC_MAIN);		break;
	case ALERT_LEVELS_SPOTTED:	Game::SetMusic(MUSIC_ALERT);		break;
	case ALERT_LEVELS_LOW:
	{
		for (int i = 0; i < mPool.SIZE; i++) if (mPool.IsActive(i) && !mPool[i].mDestroyed)
		{
			mPool[i].Alert(); 
		}
		break;
	}
	case ALERT_LEVELS_HIGH: 
	{
		mSpawnAlarm.Reset(); 

		for (int i = 0; i < mPool.SIZE; i++) if (mPool.IsActive(i) && !mPool[i].mDestroyed)
		{
			mPool[i].Alert();
		}
		break;
	}
	default: break; 
	}
}

void Soldiers::SetAlertLevel()
{
	SetAlertLevel(mType->mAlertLevel);  
}

int2 Soldiers::FindSpawnTile() const
{
	int const random = randomCardinal(); 
	for (int i = 0; i < CARDINALS_COUNT; i++)
	{
		int2 coord = FindSpawnTileOnSide((random + i) % CARDINALS_COUNT); 
		if (coord != int2(-1, -1)) return coord; 
	}
	return int2(-1, -1); 
}

void Soldiers::SpawnReinforcement()
{
	if (mPool.GetActiveCount() >= SOLDIER_COUNT) return;

	int2 coord = FindSpawnTile(); 
	if (coord == int2(-1, -1)) return; 

	Soldier& soldier = mPool[mPool.WakeObject()];
	soldier.SetPosition(TileToPixel(coord));  
	soldier.Alert(); 
	soldier.ResetHealth(); 
}

int2 Soldiers::FindSpawnTileOnSide(int const cardinal) const
{
	// TODO devise a more sophisticated approach: 
	// give it ten chances:
	int2 side = cardinalToInt2(cardinal);
	int2 const axes = axesToInt2(cardinalToAxes(cardinal));
	side += axes;  
	side = int2(side / int2(2));  
	for (int i = 0; i < 10; i++) 
	{
		int2 const coord = int2(side.x * (Tilemap::COLUMNS - 1) + axes.y * (RandomUInt() % Tilemap::COLUMNS),
								side.y * (Tilemap::ROWS - 1) + axes.x * (RandomUInt() % Tilemap::ROWS));
		float2 pos = TileToPixel(coord);
		float2 halfSize = mPool[0].GetBboxTile().GetHalfSize(); 
		TileArea area = SubPixelToTileArea(pos - halfSize, pos + halfSize); 
		if (!AABB::DetectTilemap(area)) return coord; 
	}

	return int2(-1, -1); 
}

void Soldiers::SpawnReinforcementOnSide(int const cardinal)
{
	if (mPool.GetActiveCount() >= SOLDIER_COUNT) return;

	int2 coord = FindSpawnTileOnSide(cardinal);
	if (coord == int2(-1, -1)) return;

	Soldier& soldier = mPool[mPool.WakeObject()];
	soldier.SetPosition(TileToPixel(coord));
	soldier.Alert();
	soldier.ResetHealth();
}
