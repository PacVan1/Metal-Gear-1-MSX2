#pragma once

#include "TilePalette.h"  

class Tilemap
{
public:
	static constexpr uint8_t COLUMNS	= 32;
	static constexpr uint8_t ROWS		= 24;
	static constexpr uint	 SIZE		= COLUMNS * ROWS; 

public:
	TilePalette*	tilePalette; 

private:
	uint8_t			tileIdxs[COLUMNS * ROWS]; // indices to tilepalette

public:
	Tilemap(char const* path);  
	void Render(Surface8* screen) const;  
	uint8_t GetTileState(uint idx) const; // indices to tileIdxs
	uint8_t GetTileState(int2 idx) const; // indices to tileIdxs
	void InsertMetaTile(int2 coord, MetaTile const& mt);  

private:
	bool LoadTileIndices(char const* path);   
};

static inline int2 PixelToTile(int2 coord)
{
	return int2(coord.x / TilePalette::Tile::SIZE, coord.y / TilePalette::Tile::SIZE); 
}
static inline int2 SubPixelToTile(float2 pos)
{
	return int2(floor(pos.x / TilePalette::Tile::SIZE), floor(pos.y / TilePalette::Tile::SIZE));
}
static inline int2 TileToPixel(int2 coord)
{
	return coord * TilePalette::Tile::SIZE;
}
static inline int  TileToID(int2 coord)
{
	return coord.x + coord.y * Tilemap::COLUMNS; 
}
static inline int  PixelToID(int2 pos)
{
	return TileToID(PixelToTile(pos));
}
static inline int  SubPixelToID(float2 pos)
{
	return TileToID(SubPixelToTile(pos));
}

struct TileArea
{
public:
	int2 tlCoord;
	int2 brCoord;

public:
	TileArea(int2 tlCoord, int2 brCoord) :
		tlCoord(tlCoord),
		brCoord(brCoord)
	{}
};

// tile area functions
inline TileArea SubPixelToTileArea(float2 tlCoord, float2 brCoord)
{
	return TileArea(SubPixelToTile(tlCoord), SubPixelToTile(brCoord));
}