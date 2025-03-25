#pragma once

#include "Color.h" 

class ColorPalette
{
public:
	static u8 constexpr COLOR_COUNT = 8; 
	static u8 constexpr CYCLE_COUNT = 4; // these are the amount of colors (counted from the start of the array) that are getting cycled.

public:
	Color colors[COLOR_COUNT]; 

public:
	static ColorPalette* LoadFromFile(char const* path);  

public:
	ColorPalette(Color colors[COLOR_COUNT]); 
	void Cycle(); 
};

