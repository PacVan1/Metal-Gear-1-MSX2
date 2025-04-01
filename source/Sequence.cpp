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

	flags = new SequenceFlag[flagCount]; 
	fscanf(file, "Flags: "); // scan start of array 
	for (int i = 0; i < flagCount; i++)
	{
		flags[i].animState = 1;
		if (!fscanf(file, "%u", &flags[i].position)) 
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

void Sequencer::Play(float const dt)
{
	if (!sequence) return;

	actor.SetPosition(actor.GetPosition() + CardinalToFloat2(actor.facing) * /*sequence->speed*/ 0.005f * dt);
	if (facing != GetCardinal())
	{
		actor.bbox.fPos[axis] = static_cast<float>(sequence->flags[flagIdx].position);   
		reachedFlag = true;
		NextFlag();
	}
	else
	{
		reachedFlag = false;
		reachedEnd	= false;
	}
}

int Sequencer::GetCardinal() const
{
	int flag = sequence->flags[flagIdx].position; 

	if (axis == HORIZONTAL)
	{
		return (actor.bbox.iPos.x < flag) ? EAST : WEST;
	} 
	return (actor.bbox.iPos.y < flag) ? SOUTH : NORTH; 
}

int Sequencer::GetNextAxis() const 
{
	return (axis == HORIZONTAL) ? VERTICAL : HORIZONTAL; 
}

void Sequencer::SetSequence(Sequence const* sequence)
{
	this->sequence = sequence;
	Reset(); 
}

void Sequencer::Continue() const 
{
	actor.facing = facing;
	actor.SetAnimationState(sequence->flags->animState);
	actor.SetAnimation();
}

void Sequencer::NextFlag()
{
	flagIdx	= (++flagIdx) % sequence->flagCount;
	reachedEnd = flagIdx == 0; 
	axis	= GetNextAxis();
	facing	= GetCardinal();
}

void Sequencer::Reset()
{
	flagIdx		= 0;
	reachedEnd	= false;
	reachedFlag = false;

	if (sequence)
	{
		actor.SetPosition( sequence->start); 
		axis	= sequence->startAxis;
		facing	= GetCardinal(); 
	}
}
