#pragma once

enum axis : uint8_t
{
	HORIZONTAL,
	VERTICAL,
	AXIS_COUNT
};

enum cardinals : uint8_t
{
	EAST, 
	NORTH, 
	WEST, 
	SOUTH,
	CARDINALS_COUNT,
};

extern int2 const	AXES_INT2[AXIS_COUNT];
extern float2 const AXES_FLOAT2[AXIS_COUNT];
extern int2 const	CARDINAL_INT2[CARDINALS_COUNT];
extern float2 const CARDINAL_FLOAT2[CARDINALS_COUNT];

float2	axesToFloat2(int const axes); 
float2	cardinalToFloat2(int const cardinal);
int2	axesToInt2(int const axes);
int2	cardinalToInt2(int const cardinal); 
int		int2ToCardinal(int2 const cardinal);
int		cardinalToAxes(int const cardinal);
int		randomCardinal(); 