#pragma once

#include "Cardinals.h" 

// forward declaring Actor
class Actor; 

struct Sequence
{
public:
	enum axes
	{
		HORIZONTAL, VERTICAL
	};

public:
	int2	start;
	int		startAxis;
	float	speed;
	int*	flags;
	int		flagCount;

public:
	Sequence(char const* path);  
};

class Sequencer
{
public:
	Actor&			actor;
	Sequence const*	sequence;   
	uint			flagIdx; 
	cardinals		facing;
	int				axis;

public:
				Sequencer(Actor& actor); 
	bool		Play(float const dt);
	cardinals	GetCardinal();
	int			GetNextAxis();
	void		SetSequence(Sequence const* sequence);
	void		Continue();
	void		NextFlag(); 
	void		Reset();
};
