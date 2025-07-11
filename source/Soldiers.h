#pragma once

#include "ObjectPool.h"
#include "Soldier.h" 
#include "AlertPopup.h" 

int constexpr	SOLDIER_COUNT		= 6;
float constexpr SOLDIER_SPAWN_TIME	= 3.0f;

enum alertLevels : uint8_t
{
	ALERT_LEVELS_OFF,
	ALERT_LEVELS_SPOTTED,
	ALERT_LEVELS_LOW,
	ALERT_LEVELS_HIGH
};

class Soldiers
{
public:
	AlertPopup				mAlertPopup; 
	ObjectPool<Soldier>		mPool;

private:
	SoldierTypeData const*	mType; 
	int						mAlertLevel; 
	Alarm					mSpawnAlarm;   

public:
				Soldiers(); 
	void		Update(float const dt);
	void		Render(Surface8* screen) const;  
	bool		AreDead() const;

	void		SetType(int const typeIdx);  
	void		SetAlertLevel(int const alertLevel);
	void		SetAlertLevel(); 

	inline int	GetAlertLevel() const { return mAlertLevel; }
	inline auto GetSoldierType() const { return mType; }

	int2		FindSpawnTile() const;
	void		SpawnReinforcement();
	int2		FindSpawnTileOnSide(int const cardinal) const;
	void		SpawnReinforcementOnSide(int const cardinal);
};

