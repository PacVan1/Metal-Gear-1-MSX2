#pragma once

struct Bitmap
{
	u8* indices; 
	u32 width;
	u32 height;  

	static Bitmap* LoadFromFile(char const* path);   

	Bitmap(u8* indices, u32 const width, u32 const height);
	Bitmap(u32 const width, u32 const height);  
	Bitmap(Bitmap const& other);  
	~Bitmap(); 
};