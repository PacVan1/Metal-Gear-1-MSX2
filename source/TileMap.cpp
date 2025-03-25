#include "precomp.h"
#include "TileMap.h"

#include "AABB.h"

Tilemap::Tilemap(char const* path)
{
	Files::PrintLoading(path); 
	FILE* file = Files::ReadFile(path); 
	char const* dir = Files::FindDirectory(path); 

	char relPath[128]; 
	char subPath[256]; 
	if (!fscanf(file, "TileIndices: %s\n", relPath)) 
	{
		Files::PrintFailure(path); 
		return; 
	}
	strcpy(subPath, dir); strcat(subPath, relPath); 
	if (!LoadTileIndices(subPath))
	{
		Files::PrintFailure(path); 
		return; 
	}

	uint tilePaletteIdx = 0; 
	if (!fscanf(file, "TilePalette: %d\n", &tilePaletteIdx))
	{
		Files::PrintFailure(path); 
		return; 
	}
	tilePalette = &TilePalette::palettes[tilePaletteIdx]; 

	tilePalette->sheet.sprite.palette = &colorPalette;  

	fclose(file); 
	Files::PrintSuccess(path); 
}

void Tilemap::Render(Surface8* screen) const
{
	int idx = 0; 
	for (int yy = 0; yy < ROWS; yy++) for (int xx = 0; xx < COLUMNS; xx++, idx++)
	{
		tilePalette->sheet.Render(screen, xx * TilePalette::Tile::SIZE, yy * TilePalette::Tile::SIZE, tileIdxs[idx]);
		if (GetTileState(idx) == TilePalette::Tile::SOLID)
		{
			screen->Box(xx * TilePalette::Tile::SIZE, yy * TilePalette::Tile::SIZE, xx * TilePalette::Tile::SIZE + 7, yy * TilePalette::Tile::SIZE + 7, 80);
		}
		else if (GetTileState(idx) == TilePalette::Tile::CLIMBABLE)
		{
			screen->Box(xx * TilePalette::Tile::SIZE, yy * TilePalette::Tile::SIZE, xx * TilePalette::Tile::SIZE + 7, yy * TilePalette::Tile::SIZE + 7, 162);
		}
		else if (GetTileState(idx) == TilePalette::Tile::SWIMABLE)  
		{
			screen->Box(xx * TilePalette::Tile::SIZE, yy * TilePalette::Tile::SIZE, xx * TilePalette::Tile::SIZE + 7, yy * TilePalette::Tile::SIZE + 7, 47);
		}
	}
}

/// returns tile state, but will not do bounds check
uint8_t Tilemap::GetTileState(uint idx) const 
{
	return tilePalette->tiles[tileIdxs[idx]].state;  
}

/// returns tile state and does bounds check
uint8_t Tilemap::GetTileState(int2 coord) const 
{
	if (AABB::Detect(coord, COLUMNS - 1, 0, 0, ROWS - 1)) 
	{
		return tilePalette->tiles[tileIdxs[TileToID(coord)]].state;
	}
	return TilePalette::Tile::NONE;
}

bool Tilemap::LoadTileIndices(char const* path) 
{
	FILE* file = Files::ReadFile(path); 

	char* csv = new char[256];  // maximum estimated size 
	int* ids = new int[COLUMNS];  
	uint addr = 0;  
	for (int i = 0; i < ROWS; i++)  
	{ 
		Files::GetLine(file, csv, 256); // estimated largest size     
		Files::ParseCSVToInt(csv, ids, COLUMNS);  
		for (int j = 0; j < COLUMNS; j++) 
		{
			tileIdxs[addr] = ids[j]; 
			addr++;  
		}
	}
	delete[] csv;  
	delete[] ids; 

	fclose(file);
	return true; 
}
