#include "precomp.h"
#include "Scene.h"

Scene::Scene() :
	mItems(nullptr),
	mItemCount(0),
	mSoldierCount(0),
	mPassageCount(0),
	mSoldierTypeIdx(SOLDIER_TYPES_GRAY),
	mOwnTilemap(false)
{}

Scene::Scene(char const* path) :
	mItems(nullptr),
	mItemCount(0),
	mSoldierCount(0),
	mPassageCount(0),
	mSoldierTypeIdx(SOLDIER_TYPES_GRAY),
	mOwnTilemap(false)
{
	Files::PrintLoading(path);
	FILE* file = Files::ReadFile(path);
	char const* dir = Files::FindDirectory(path);

	char relPath[128];
	char subPath[256];
	if (!fscanf(file, "Tilemap: %s\n", relPath))
	{
		Files::PrintFailure(path);
		return;
	}
	strcpy(subPath, dir); strcat(subPath, relPath);
	mTilemap = new Tilemap(subPath);
	if (mTilemap) mOwnTilemap = true; 

	if (!fscanf(file, "SoldierCount: %d\n", &mSoldierCount))
	{
		Files::PrintFailure(path);
		return;
	}
	mPaths = new Sequence*[mSoldierCount];

	if (!fscanf(file, "Paths: %d\n", &mSoldierCount))
	{
		Files::PrintFailure(path);
		return;
	}
	for (int i = 0; i < mSoldierCount; i++)
	{
		if (!fscanf(file, "%s\n", relPath))
		{
			Files::PrintFailure(relPath); 
			return;
		}
		strcpy(subPath, dir); strcat(subPath, relPath);
		Sequence* sequence = new Sequence(subPath);  
		mPaths[i] = sequence;    
	}

	fclose(file);
	Files::PrintSuccess(path);
}

Scene::~Scene()
{
	//if (mOwnTilemap) delete mTilemap; 
	// delete paths
	// delete items
}

void Scene::Update(float const dt)
{
	for (int i = 0; i < mPassageCount; i++)
	{
		mPassages[i]->TryEnter(); 
	}
}

void Scene::Render(Surface8* screen) const
{
	mTilemap->Render(screen);  

	for (int i = 0; i < mItemCount; i++)
	{
		if (!mItems[i].mPickedUp) 
		{
			mItems[i].Render(screen);  
		}
	}

	for (int i = 0; i < mPassageCount; i++)
	{
		mPassages[i]->Render(screen); 
	}
}

SceneTracker::SceneTracker() :
	mScene(nullptr),
	mCoord(0, 0),
	mType(0),
	mIdx(0)
{}
