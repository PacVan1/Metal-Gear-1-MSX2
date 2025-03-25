#include "precomp.h"
#include "Cardinals.h" 

int2 const CARDINAL_INT2[cardinals::COUNT] =
{
	{ 1, 0 },
	{ 0,-1 },
	{-1, 0 },
	{ 0, 1 }
};

float2 const CARDINAL_FLOAT2[cardinals::COUNT] = 
{
	{ 1.0, 0.0 },
	{ 0.0,-1.0 },
	{-1.0, 0.0 },
	{ 0.0, 1.0 }
};

int2 CardinalToInt2(int cardinal) 
{
	return CARDINAL_INT2[cardinal]; 
}

float2 CardinalToFloat2(int cardinal)
{
	return CARDINAL_FLOAT2[cardinal];  
}

int Int2ToCardinal(int2 cardinal)
{
	for (int i = 0; i < cardinals::COUNT; i++) 
	{
		if (cardinal == CARDINAL_INT2[i])
 		{
			return i; 
		}
	}
}
