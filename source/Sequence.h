#pragma once

class Actor; 

struct SequenceFlag
{
	int mPos;
	int mAnimState; 
};

struct Sequence
{
	SequenceFlag*	mFlags;
	int2			mStart;
	float			mSpeed;
	int				mStartAxis;
	int				mFlagCount;

	Sequence(char const* path);  
};

class Sequencer
{
private:
	Actor*			mActor;
	Sequence const*	mSequence;   
	int				mFlagIdx; 
	int				mFacing;
	int				mAxis;
	bool			mReachedFlag;
	bool			mReachedEnd; 

public:
				Sequencer(Actor* actor); 
	void		Play(float const dt);
	int			GetCardinal() const;
	int			GetNextAxis() const;
	void		SetSequence(Sequence const* sequence);
	void		Continue() const;
	void		NextFlag(); 
	void		Reset();

	inline bool	HasReachedFlag() const	{ return mReachedFlag; }
	inline bool	HasReachedEnd() const	{ return mReachedEnd; }
};
