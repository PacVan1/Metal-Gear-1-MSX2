#include "precomp.h"
#include "Scene.h"

Scene::Scene(char const* path)
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
	tilemap = new Tilemap(subPath);

	if (!fscanf(file, "SoldierCount: %d\n", &soldierCount))
	{
		Files::PrintFailure(path);
		return;
	}
	paths		= new Sequence*[soldierCount];

	if (!fscanf(file, "Paths: %d\n", &soldierCount))
	{
		Files::PrintFailure(path);
		return;
	}
	for (int i = 0; i < soldierCount; i++)
	{
		if (!fscanf(file, "%s\n", relPath))
		{
			Files::PrintFailure(path);
			return;
		}
		strcpy(subPath, dir); strcat(subPath, relPath);
		paths[i] = new Sequence(subPath);  
	}

	fclose(file);
	Files::PrintSuccess(path);
}
