#pragma once

enum cardinals
{
	EAST, 
	NORTH, 
	WEST, 
	SOUTH,
	COUNT,
};

extern int2 const	CARDINAL_INT2[cardinals::COUNT];  
extern float2 const CARDINAL_FLOAT2[cardinals::COUNT];   

int2	CardinalToInt2(int cardinal); 
float2	CardinalToFloat2(int cardinal);
int		Int2ToCardinal(int2 cardinal);