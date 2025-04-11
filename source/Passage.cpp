#include "precomp.h"
#include "Passage.h"

#include "Game.h" 

void Passage::Connect(PassageSharedData* sharedData, Passage* passage1, Passage* passage2)
{
	passage1->other = passage2;
	passage2->other = passage1;
	passage1->sharedData = sharedData;
	passage2->sharedData = sharedData;
}

Passage::Passage(PassageProps const& props) :
	mBbox(&mPosF, TileToPixel({ props.dataLocked.columns, props.dataLocked.rows }), 0),
	props(props) 
{}

void Passage::SetPosition(int2 const tileCoord)
{
	mTileCoord = tileCoord;
	mPosI = TileToPixel(tileCoord); 
	mPosF = static_cast<float2>(mPosI); 
	mBbox.Update();
	spawnPosition = mPosI + props.spawnPosition;   
}

void Passage::Render(Surface8* screen) const
{
	if (sharedData->mUnlocked) mBbox.Render(screen, 30);
	else mBbox.Render(screen, 162);
}

void Passage::TryEnter() const
{
	if (!sharedData->mUnlocked) return; 

	if (AABB::Detect(Game::sPlayer.GetBboxTile(), mBbox)) 
	{
		//Game::SwitchScene(other->scene);
		Game::SwitchScene(other->mSceneIdx); 
		Game::sPlayer.SetPosition(other->spawnPosition);
	}
}