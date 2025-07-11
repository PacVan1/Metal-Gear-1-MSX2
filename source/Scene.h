#pragma once

#include "TileMap.h"
#include "Sequence.h"
#include "Item.h"
#include "Passage.h" 
#include "SoldierTypes.h" 

enum sceneType : uint8_t
{
	SCENE_TYPES_ADJACENT,
	SCENE_TYPES_EXTRA
};

class Scene
{
public:
	// temporary:
	inline static ColorPalette8 sColorPalette1 = ColorPalette8("assets/color_palettes/outer_heaven.cpalette");
	inline static ColorPalette8 sColorPalette2 = ColorPalette8("assets/color_palettes/outside2.cpalette");

public: 
	Tilemap*		mTilemap;
	Sequence**		mPaths;
	ItemObject*		mItems;
	Passage**		mPassages;
	int				mItemCount; 
	int				mSoldierCount;
	int				mPassageCount;
	int				mSoldierTypeIdx;  
	ColorPalette8 const* mColorPalette = nullptr;

private:
	bool			mOwnTilemap; 

public: 
	Scene();
	Scene(char const* path);
	~Scene(); 
	void	Update(float const dt);
	void	Render(Surface8* screen) const;

	inline Sequence const* GetPath(int const idx) const { return mPaths[idx]; }
};

struct SceneTracker
{
	Scene*	mScene; 
	int2	mCoord;
	int		mType;
	int		mIdx = 10000;

public:
	SceneTracker(); 
};
