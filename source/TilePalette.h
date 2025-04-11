#pragma once

#include "SpriteSheet.h" 

struct TilePalette
{
	struct Tile 
	{
		enum states
		{
			NONE, SOLID, CLIMBABLE, SWIMABLE, COUNT 
		};

		static constexpr uint8_t SIZE = 8;

		uint8_t	state; // look at states  
	};

	static TilePalette palettes[1];

	SpriteSheet	sheet;
	Tile		tiles[256];
	uint8_t		tileCount;  

	static void InitPalettes();  

			TilePalette() = default; 
			TilePalette(char const* path); 
	bool	LoadTileStates(char const* path);  
};

struct MetaTile
{
	uint8_t*	tile = nullptr;
	uint8_t		columns = 0;
	uint8_t		rows = 0; 
};
