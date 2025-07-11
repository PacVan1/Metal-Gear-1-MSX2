#pragma once

#include "SpriteSheet.h" 

class AlertPopup
{
private:
	SpriteSheet		mSpriteSheet = SpriteSheet("assets/spritesheets/alert_cloud/alert_cloud.sheet");
	ColorPalette8	mPalette = ColorPalette8("assets/color_palettes/alert_cloud.cpalette");
	int2			mPosI;
	int				mFrame; 

public:
			AlertPopup();
	void	Render(Surface8* screen) const;
	void	SetPosition(int2 const position); 
};

