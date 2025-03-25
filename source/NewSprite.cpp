#include "precomp.h"
#include "NewSprite.h"

#include "Settings.h"

NewSprite::NewSprite(Bitmap& bitmap) : 
	bitmap(bitmap)  
{}

void NewSprite::Render(Surface* screen, s32 x, s32 y)
{
	// do not draw if sprite doesn't have color 
	if (!colorPalette) return;

	if (y < -(s32)bitmap.height || y >(screen->height)) return; 
	if (x < -(s32)bitmap.width || x >(screen->width)) return; 

	s32 x1 = x, x2 = x + bitmap.width;
	s32 y1 = y, y2 = y + bitmap.height;
	u8* src = bitmap.indices;
	if (x1 < 0) src += -x1, x1 = 0;
	if (x2 > screen->width) x2 = screen->width;
	if (y1 < 0) src += -y1 * bitmap.width, y1 = 0;
	if (y2 > screen->height) y2 = screen->height;
	u32* dst = screen->pixels;

	// check if sprite is not flat 
	if (x2 <= x1 && y2 <= y1) return;

	u32 dstAddr = x1 + y1 * screen->width;
	u32 w = x2 - x1;
	u32 h = y2 - y1;
	for (s32 yy = 0; yy < h; yy++)
	{
		for (s32 xx = 0; xx < w; xx++)
		{
			u32 c = colorPalette->colors[src[xx]].value;
			if (c & 0xffffff)
			{
				dst[dstAddr + xx] = c;
			}
		}
		dstAddr += screen->width;
		src += bitmap.width;
	}

#if SPRITE_MODE      
	screen->Box(x, y, x + bitmap.width, y + bitmap.height, 0x00ff00);  
#endif
}
