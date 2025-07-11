#pragma once

class Tile
{
public:
	static constexpr uint SIZE = 8;

public:
	Sprite*	sprite;  
	bool	solid;

public:
	Tile(Sprite* sprite, bool solid);
};

