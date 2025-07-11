#include "precomp.h"
#include "Sequence.h"

#include "Cardinals.h" 
#include "Actor.h"

Sequence::Sequence(char const* path) 
{
	Files::PrintLoading(path); 
	FILE* file = Files::ReadFile(path); 

	if (!fscanf(file, "Start: %d %d\n", &mStart.x, &mStart.y))
	{
		Files::PrintFailure(path);
		return;
	}
	if (!fscanf(file, "StartAxis: %d\n", &mStartAxis))
	{
		Files::PrintFailure(path);
		return;
	}
	if (!fscanf(file, "Speed: %f\n", &mSpeed))
	{
		Files::PrintFailure(path);
		return;
	}
	if (!fscanf(file, "FlagCount: %d\n", &mFlagCount))
 	{
		Files::PrintFailure(path);
		return;
	}

	mFlags = new SequenceFlag[mFlagCount]; 
	fscanf(file, "Flags: "); // scan start of array 
	for (int i = 0; i < mFlagCount; i++)
	{
		mFlags[i].mAnimState = 1; 
		if (!fscanf(file, "%u", &mFlags[i].mPos)) 
		{
			Files::PrintFailure(path); 
			return;
		}
	}
	fscanf(file, "\n");  // next line   

	fclose(file);
	Files::PrintSuccess(path);
}

Sequencer::Sequencer(Actor* actor) :
	mActor(actor),
	mSequence(nullptr)
{
	Reset();
}

void Sequencer::Play(float const dt)
{
	if (!mSequence) return;

	mActor->SetPosition(mActor->GetPosition() + cardinalToFloat2(mFacing) * mSequence->mSpeed * dt);
	if (mFacing != GetCardinal())
	{
		mReachedFlag = true;
		NextFlag();
	}
	else
	{
		mReachedEnd	= false;
		mReachedFlag = false;
	}
}

int Sequencer::GetCardinal() const
{
	int const flag = mSequence->mFlags[mFlagIdx].mPos; 

	if (mAxis == HORIZONTAL)
	{
		return (mActor->GetPositionInt().x < flag) ? EAST : WEST;
	} 
	return (mActor->GetPositionInt().y < flag) ? SOUTH : NORTH; 
}

int Sequencer::GetNextAxis() const 
{
	return (mAxis == HORIZONTAL) ? VERTICAL : HORIZONTAL; 
}

void Sequencer::SetSequence(Sequence const* sequence)
{
	mSequence = sequence; 
	Reset(); 
}

void Sequencer::Continue() const 
{ 
	mActor->SetAnimationState(mSequence->mFlags[mFlagIdx].mAnimState);
	mActor->SetFacing(mFacing); 
}

void Sequencer::NextFlag()
{
	mFlagIdx	= (++mFlagIdx) % mSequence->mFlagCount;
	mReachedEnd = mFlagIdx == 0; 
	mAxis	= GetNextAxis();
	mFacing	= GetCardinal();
}

void Sequencer::Reset()
{
	mFlagIdx		= 0;
	mReachedEnd		= false;
	mReachedFlag	= false;

	if (mSequence)
	{
		mActor->SetPosition(mSequence->mStart); 
		mAxis	= mSequence->mStartAxis;
		mFacing	= GetCardinal(); 
	}
}
