#include "precomp.h"
#include "Image.h"

#include "Files.h" 

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#include "lib/stb_image.h" 

Image* Image::LoadFromFile(char const* path)
{
	Files::PrintLoading(path);   
	// use stb_image to load the image file
	int width, height, channels;  
	uint8_t* data = stbi_load(path, &width, &height, &channels, 0); 
	if (!data)
	{
		Files::PrintFailure(path);
		return NULL; // load failed
	}
	// convert uint8_t with color channels to uint32_t:
	int const size = width * height; 
	uint32_t* buffer = (uint32_t*)MALLOC64(size * sizeof(uint32_t)); 
	if (buffer) 
	{
		if (channels == 1) for (int i = 0; i < size; i++) /* greyscale */ 
		{
			uint8_t const p = data[i];
			buffer[i] = CalcColor(p, p, p);
		}
		else for (int i = 0; i < size; i++)
		{
			uint32_t colorIdx = i * channels; 
			buffer[i] = CalcColor(data[colorIdx], data[colorIdx + 1], data[colorIdx + 2]);
		}
	}

	// free stb_image data
	stbi_image_free(data); 
	Image* image = new Image(buffer, width, height); 
	Files::PrintSuccess(path);  
	return image;  
}

Image::Image(uint32_t* buffer, int width, int height) :  
	data(buffer),  
	width(width), 
	height(height)
{}

Image::~Image()
{
	FREE64(data);   
}
