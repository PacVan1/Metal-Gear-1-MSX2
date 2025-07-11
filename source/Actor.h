#pragma once

#include "IDamageable.h" 
#include "AnimatedSprite.h" 
#include "Sequence.h"

uint8_t constexpr BBOX_TILE_COLOR = 120;
uint8_t constexpr BBOX_PIXEL_PERFECT_COLOR = 30;

class Actor : public IDamageable
{
public:
	AnimatedSprite		mDisplay;
	bool				mHit = false;  
	AABB				mBbox; 
	AABB				mBboxHead;
	AABB				mBboxBody; 

protected:
	Sequencer			mSequencer;
	AABB				mBboxTile; 
	float2				mPosF;
	int2				mPosI; 
	float				mSpeed; 
	int					mDirAnimCount; 
	int					mAnimState;
	int					mBehavior;   
	int					mFacing;

public:
						Actor(); 
						Actor(AnimationSpriteSheet& spriteSheet, int const dirAnimCount);
	virtual				~Actor() = default;

	virtual void		Render(Surface8* screen) const; 

	void				SetPosition(float2 const position);  
	void				SetBehavior(int const behavior);  
	void				SetFacing(int const facing);
	void				SetState(int const behavior, int const facing);
	void				SetPath(Sequence const* sequence);  
	virtual void		SetSpriteSheet(AnimationSpriteSheet& spriteSheet, int const dirAnimCount);

	inline void			SetAnimationState(int const animState) { mAnimState = animState; }

	inline AABB const&	GetBbox() const			{ return mBbox; }
	inline AABB const&	GetBboxTile() const		{ return mBboxTile; }
	inline float2		GetPosition() const		{ return mPosF; }
	inline int2			GetPositionInt() const	{ return mPosI; }
	inline float		GetSpeed() const		{ return mSpeed; }
	inline int			GetFacing() const		{ return mFacing; }
	inline int			GetBehavior() const		{ return mBehavior; }
	inline int			GetAnimState() const	{ return mAnimState; }

	// collision:
	bool				Detect(AABB const& bbox) const;
	bool				DetectGrouped(AABB const& bbox) const;
	bool				DetectPixel(Actor const& other);
	bool				DetectPixel(Actor const& other, AABB const& bboxOther) const;
	bool				DetectPixel(AABB const& bbox) const; 
	bool				DetectPixel(AreaInt const& overlap) const;
	static bool			DetectPixel(Actor const& actor, AABB const& bboxActor, AABB const& bbox);
	static bool			DetectPixel(Actor const& actor1, Actor const& actor2, AABB const& bbox1, AABB const& bbox2); 
	static bool			DetectPixel(Actor const& actor1, Actor const& actor2, AreaInt const& overlap); 

	virtual void		Update(float const dt) = 0;

protected: 
	void				Move(float const dt);
	void				InitState();
	void				SetAnimation();
	int					GetAnimationIdx() const;  
	virtual void		SetAnimationState() = 0; 
};
