#pragma once

#include "Cardinals.h" 

// forward declaring Actor
class Actor; 

enum axis : u8 
{
	HORIZONTAL,
	VERTICAL
};

struct SequenceFlag
{
	int position;
	int animState; 
};

struct Sequence
{
	int2			start;
	int				startAxis;
	float			speed;
	SequenceFlag*	flags;
	int				flagCount;

	Sequence(char const* path);  
};

class Sequencer
{
public:
	Actor&			actor;
	Sequence const*	sequence;   
	uint			flagIdx; 
	int				facing;
	int				axis;

private:
	bool			reachedFlag;
	bool			reachedEnd; 

public:
			Sequencer(Actor& actor); 
	void	Play(float const dt);
	int		GetCardinal() const;
	int		GetNextAxis() const;
	void	SetSequence(Sequence const* sequence);
	void	Continue() const;
	void	NextFlag(); 
	void	Reset();

	bool	HasReachedFlag() const { return reachedFlag; }
	bool	HasReachedEnd() const { return reachedEnd; }
};
