#include "precomp.h"
#include "Cardinals.h" 

int2 const AXES_INT2[AXIS_COUNT] =
{
	{ 1, 0 },
	{ 0,1 }
};

float2 const AXES_FLOAT2[AXIS_COUNT] =
{
	{ 1.0, 0.0 },
	{ 0.0, 1.0 }
};

int2 const CARDINAL_INT2[CARDINALS_COUNT] =
{
	{ 1, 0 },
	{ 0,-1 },
	{-1, 0 },
	{ 0, 1 }
};

float2 const CARDINAL_FLOAT2[CARDINALS_COUNT] = 
{
	{ 1.0, 0.0 },
	{ 0.0,-1.0 },
	{-1.0, 0.0 },
	{ 0.0, 1.0 }
};

int2 axesToInt2(int const axes)
{
	return AXES_INT2[axes]; 
}

float2 axesToFloat2(int const axes)
{
	return AXES_FLOAT2[axes];
}

int2 cardinalToInt2(int const cardinal)
{
	return CARDINAL_INT2[cardinal]; 
}

float2 cardinalToFloat2(int const cardinal)
{
	return CARDINAL_FLOAT2[cardinal];  
}

int cardinalToAxes(int const cardinal)
{
	return cardinal % 2; 
}

int randomCardinal()
{
	return static_cast<int>(RandomUInt() % CARDINALS_COUNT);
}

int int2ToCardinal(int2 const cardinal)
{
	for (int i = 0; i < CARDINALS_COUNT; i++)
	{
		if (cardinal == CARDINAL_INT2[i])
 		{
			return i; 
		}
	}
	return 0;
}
