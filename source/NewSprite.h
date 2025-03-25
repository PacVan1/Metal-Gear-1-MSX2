#pragma once

#include "Bitmap.h" 
#include "ColorPalette.h" 

class NewSprite
{
public:
	Bitmap const		bitmap; 
	ColorPalette const* colorPalette = NULL;  

public:
	NewSprite(Bitmap& bitmap);
	void Render(Surface* screen, s32 x, s32 y);  
};

