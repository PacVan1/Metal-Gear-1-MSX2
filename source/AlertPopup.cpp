#include "precomp.h"
#include "AlertPopup.h"

#include "Soldiers.h" 

AlertPopup::AlertPopup() :
	mSpriteSheet("assets/spritesheets/alert_cloud/alert_cloud.sheet"),
	mPalette("assets/color_palettes/alert_cloud.cpalette"),
	mPosI(0),
	mFrame(0)
{
	mSpriteSheet.palette = &mPalette; 
}

void AlertPopup::Render(Surface8 * screen) const
{
	mSpriteSheet.RenderFrame(screen, mPosI.x, mPosI.y, mFrame);  
}

void AlertPopup::SetPosition(int2 const position)
{
	mPosI = position; 
}
