#include "precomp.h" 
#include "Bitmap.h" 

#include "Files.h" 

#include <iostream>

Bitmap* Bitmap::LoadFromFile(char const* path)
{
	FILE* file = Files::StartLoading(path);  

	u8* indices; 
	s32 width;
	s32 height; 

	if (!fscanf(file, "%d %d\n", &width, &height))
	{
		Files::PrintFailedToLoad(path); 
		fclose(file); 
		return NULL;
	}
	indices = new u8[width * height]; 

	int addr = 0; 
	for (int yy = 0; yy < height; yy++)  
	{
		for (int xx = 0; xx < width; xx++, addr++) 
		{
			if (!fscanf(file, "%d", &indices[addr]))
			{
				Files::PrintFailedToLoad(path);  
				delete[] indices; 
				fclose(file); 
				return NULL;
			}
		}
		fscanf(file, "\n");
	}
	fclose(file);

	Files::PrintLoadedSuccesfully(path);  
	return new Bitmap(indices, width, height); 
}

Bitmap::Bitmap(u8* indices, u32 const width, u32 const height) : 
	indices(indices), 
	width(width), 
	height(height)
{}

Bitmap::Bitmap(u32 const width, u32 const height) : 
	indices((u8*)calloc(width* height, sizeof(u8))),
	width(width), 
	height(height)
{}

#include <cstring> 

Bitmap::Bitmap(Bitmap const& other) : 
	width(other.width), 
	height(other.height) 
{
	u32 size = other.width * other.height;
	indices = new u8[size]; 
	memcpy(indices, other.indices, size * sizeof(u8));   
}

Bitmap::~Bitmap()
{
	if (indices)
	{
		delete[] indices; 
	}  
}
