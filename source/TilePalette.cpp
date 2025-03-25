#include "precomp.h"
#include "TilePalette.h"

TilePalette TilePalette::palettes[1];

void TilePalette::InitPalettes()
{
	palettes[0] = TilePalette("assets/tile_palettes/outer_heaven/outer_heaven.tpalette");
}

TilePalette::TilePalette(char const* path)
{
	Files::PrintLoading(path);
	FILE* file		= Files::ReadFile(path);
	char const* dir = Files::FindDirectory(path);

	char relPath[128];
	char subPath[256];
	// reading spritesheet: 
	if (!fscanf(file, "SpriteSheet: %s\n", relPath))
	{
		Files::PrintFailure(path);
		return;
	}
	strcpy(subPath, dir); strcat(subPath, relPath); 
	sheet = SpriteSheet(subPath); 
	tileCount = sheet.frameCount;  

	// reading tile states:
	if (!fscanf(file, "TileStates: %s\n", relPath))
	{
		Files::PrintFailure(path);
		return;
	}
	strcpy(subPath, dir); strcat(subPath, relPath);
	if (!LoadTileStates(subPath))
	{
		Files::PrintFailure(path); 
		return; 
	} 

	fclose(file);
	Files::PrintSuccess(path);
}

bool TilePalette::LoadTileStates(char const* path)
{
	FILE* file = Files::ReadFile(path); 

	for (int i = 0; i < tileCount; i++) 
	{
		if (!fscanf(file, "%u", &tiles[i].state))
		{ 
			return false;
		}
	}

	fclose(file); 
	return true; 
}
