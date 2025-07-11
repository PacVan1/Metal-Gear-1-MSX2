#pragma once

struct Image
{
	uint32_t*	data;
	uint32_t	width;
	uint32_t	height;

	static Image* LoadFromFile(char const* path);  

	Image(uint32_t* buffer, int width, int height); 
	~Image(); // responsible for freeing memory
};

