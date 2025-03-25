#include "precomp.h"
#include "ColorPalette.h"

#include "Files.h"

ColorPalette* ColorPalette::LoadFromFile(char const* path)
{
	FILE* file = Files::StartLoading(path); 

	Color colors[COLOR_COUNT];
	u32 r, g, b; // u32 because using u8(unsigned chars) with fscanf() throws runtime errors     

	for (int i = 0; i < COLOR_COUNT; i++)
	{
		// TODO: make a function for getting three uint8_t values from a file:
		if (!fscanf(file, "%d %d %d", &r, &g, &b))
		{
			Files::PrintFailedToLoad(path); 
			return NULL; 
		}
		colors[i] = Color(r, g, b);
	}

	fclose(file);
	Files::PrintLoadedSuccesfully(path); 

	return new ColorPalette(colors);
}

ColorPalette::ColorPalette(Color colors[COLOR_COUNT])
{
	for (s32 i = 0; i < COLOR_COUNT; i++)
	{
		this->colors[i] = colors[i];
 	}
}

void ColorPalette::Cycle()
{
	Color tmp = colors[0];
	for (s32 i = 1; i < CYCLE_COUNT; i++)
	{
		colors[i - 1] = colors[i];
	}
	colors[CYCLE_COUNT - 1] = tmp; 
}
