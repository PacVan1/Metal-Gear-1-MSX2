#pragma once

struct Color : uint3 
{
	u32 value; 

	static u32 ComputeValue(u8 r, u8 g, u8 b);  

	Color(u8 r = 255, u8 g = 255, u8 b = 255);  
};

