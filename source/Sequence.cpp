#include "precomp.h"
#include "Sequence.h"

#include "Files.h"
#include "Actor.h"

Sequence::Sequence(char const* path) 
{
	Files::PrintLoading(path); 
	FILE* file = Files::ReadFile(path); 

	if (!fscanf(file, "Start: %d %d\n", &start.x, &start.y))
	{
		Files::PrintFailure(path);
		return;
	}
	if (!fscanf(file, "StartAxis: %d\n", &startAxis))
	{
		Files::PrintFailure(path);
		return;
	}
	if (!fscanf(file, "Speed: %f\n", &speed))
	{
		Files::PrintFailure(path);
		return;
	}
	if (!fscanf(file, "FlagCount: %d\n", &flagCount))
 	{
		Files::PrintFailure(path);
		return;
	}

	flags = new int[flagCount]; 
	fscanf(file, "Flags: "); // scan start of array 
	for (int i = 0; i < flagCount; i++)
	{
		if (!fscanf(file, "%u", &flags[i]))
		{
			Files::PrintFailure(path); 
			return;
		}
	}
	fscanf(file, "\n");  // next line   

	fclose(file);
	Files::PrintSuccess(path);
}

Sequencer::Sequencer(Actor& actor) :
	actor(actor),
	sequence(nullptr)
{
	Reset();
}

bool Sequencer::Play(float const dt)
{
	if (!sequence) return false;

	actor.SetPosition(actor.bbox.fPos + CardinalToFloat2(actor.facing) * sequence->speed * dt);
	if (facing != GetCardinal())
	{
		actor.bbox.fPos[axis] = (float)sequence->flags[flagIdx]; 
		NextFlag();
		return true; 
	}
	return false; 
}

cardinals Sequencer::GetCardinal()
{
	int flag = sequence->flags[flagIdx];

	if (axis == Sequence::HORIZONTAL)
	{
		return (actor.bbox.iPos.x < flag) ? cardinals::EAST : cardinals::WEST;
	} 
	return (actor.bbox.iPos.y < flag) ? cardinals::SOUTH : cardinals::NORTH; 
}

int Sequencer::GetNextAxis() 
{
	return (axis == Sequence::HORIZONTAL) ? Sequence::VERTICAL : Sequence::HORIZONTAL; 
}

void Sequencer::SetSequence(Sequence const* sequence)
{
	this->sequence = sequence;
	Reset(); 
}

void Sequencer::Continue() 
{
	actor.facing = facing;
	actor.SetAnimation();
}

void Sequencer::NextFlag()
{
	flagIdx	= (++flagIdx) % sequence->flagCount;
	axis	= GetNextAxis();
	facing	= GetCardinal();
}

void Sequencer::Reset()
{
	flagIdx	= 0;

	if (sequence)
	{
		actor.SetPosition( sequence->start); 
		axis			= sequence->startAxis;
		facing			= GetCardinal(); 
	}
}
