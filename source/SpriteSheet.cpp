#include "precomp.h"
#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(char const* path) 
{
	Files::PrintLoading(path); 
	FILE* file = Files::ReadFile(path);
	char const* dir = Files::FindDirectory(path);  
	
	char relPath[128];   
	char subPath[256];
	if (!fscanf(file, "Sprite: %s\n", relPath))
	{
		Files::PrintFailure(path); 
		return;
	} 
	strcpy(subPath, dir); strcat(subPath, relPath);   
	new (this) Sprite(subPath);  
	
	if (!fscanf(file, "FrameCount: %d", &mFrameCount))
	{
		Files::PrintFailure(path); 
		return; 
	}
	mFrameWidth = width / mFrameCount;
	
	fclose(file);  
	Files::PrintSuccess(path);  
}

void SpriteSheet::RenderFrame(Surface8* screen, int x, int y, uint const frame) const
{
	// do not draw if sprite doesn't have color:
	if (!palette) return;

	// check if the sprite is within the bounds of the screen:
	if (y < -height || y >(screen->height)) return; 
	if (x < -mFrameWidth || x >(screen->width)) return; 

	int x1 = x, x2 = x + mFrameWidth;
	int y1 = y, y2 = y + height; 
	uint8_t* src = pixels + frame * mFrameWidth;
	if (x1 < 0) src += -x1, x1 = 0;
	if (x2 > screen->width) x2 = screen->width;
	if (y1 < 0) src += -y1 * width, y1 = 0;
	if (y2 > screen->height) y2 = screen->height;
	uint8_t* dst = screen->pixels;

	// check if sprite is not flat:
	if (x2 <= x1 && y2 <= y1) return;

	uint dstAddr = x1 + y1 * screen->width;
	uint w = x2 - x1;
	uint h = y2 - y1;
	for (int yy = 0; yy < h; yy++)
	{
		for (int xx = 0; xx < w; xx++)
		{
			// only render if the pixel is not black (translucent):  
 			u8 c = palette->indices[src[xx]];
			if (c & 0xffffff)
			{
				dst[dstAddr + xx] = c;
			}
		}
		dstAddr += screen->width;
		src += width;
	}

#if SPRITE_MODE 
	// minus 1 to make the box inclusive of the sprite:
	screen->Box(x, y, x + mFrameWidth - 1, y + height - 1, 80);
#endif
}

AnimationSpriteSheet::AnimationSpriteSheet(char const* path)
{
	Files::PrintLoading(path); 
	FILE* file = Files::ReadFile(path);
	char const* dir = Files::FindDirectory(path);  
	
	char relPath[128];   
	char subPath[256];
	if (!fscanf(file, "Sprite: %s\n", relPath))
	{
		Files::PrintFailure(path); 
		return;
	} 
	strcpy(subPath, dir); strcat(subPath, relPath);   
	new (this) Sprite(subPath);    
	
	if (!fscanf(file, "AnimSet: %s\n", relPath))
	{
		Files::PrintFailure(path); 
		return;
	} 
	strcpy(subPath, dir); strcat(subPath, relPath); 
	mAnimSet = AnimationSet(subPath); 
	
	if (!fscanf(file, "FrameCount: %d", &mFrameCount)) 
	{
		Files::PrintFailure(path); 
		return; 
	}
	mFrameWidth = width / mFrameCount; 
	
	fclose(file);  
	Files::PrintSuccess(path);  
}
