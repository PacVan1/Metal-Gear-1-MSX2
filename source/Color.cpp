#include "precomp.h"
#include "Color.h"

u32 Color::ComputeValue(u8 r, u8 g, u8 b) 
{
	return r << 16 | g << 8 | b;
}

Color::Color(u8 r, u8 g, u8 b) :
	uint3(r, g, b), 
	value(ComputeValue(r, g, b)) 
{}
