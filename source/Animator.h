#pragma once

#include "Animation.h" 

class AnimatedSprite;

class Animator
{
public:
	int					mRate = 200;

private: 
	AnimatedSprite*		mDisplay;  
	Animation const*	mAnim; // for readability and efficiency
	float				mTime;
	int					mAnimIdx; 
	bool				mFinished;

public:
				Animator() = default;  
				Animator(AnimatedSprite* display);
	void		Play(float const dt);
	void		SetAnimation(int const idx); 
	void		Reset();

	inline int	GetAnimIdx() const { return mAnimIdx; }
	inline bool IsFinished() const { return mFinished; }
};

