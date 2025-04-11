#include "precomp.h"
#include "Actor.h"

Actor::Actor() :
	mDisplay(),
	mSequencer(this),
	mBbox(&mPosF),
	mBboxTile(&mPosF),
	mPosF(0.0f),
	mPosI(0),
	mSpeed(0.0f),
	mDirAnimCount(0),
	mAnimState(0),
	mBehavior(0),
	mFacing(EAST),
	mBboxHead(&mPosF),
	mBboxBody(&mPosF) 
{}

Actor::Actor(AnimationSpriteSheet& spriteSheet, int const dirAnimCount) :
	mSequencer(this),
	mBbox(&mPosF),
	mBboxTile(&mPosF),
	mSpeed(0.0f),
	mDirAnimCount(0),
	mAnimState(0),
	mBehavior(0),
	mFacing(EAST),
	mBboxHead(&mPosF),
	mBboxBody(&mPosF)
{
	SetSpriteSheet(spriteSheet, dirAnimCount);  
}

void Actor::Render(Surface8* screen) const
{
	mBbox.Render(screen);  
	mBboxTile.Render(screen, BBOX_TILE_COLOR);
	mBboxBody.Render(screen);
	mBboxHead.Render(screen);
	mDisplay.Render(screen, static_cast<int>(mBbox.GetTopLeft().x), static_cast<int>(mBbox.GetTopLeft().y));
} 

bool Actor::Detect(AABB const& bbox) const
{
	if (AABB::Detect(mBbox, bbox))
	{
		return	DetectPixel(*this, mBboxBody, bbox) ||
				DetectPixel(*this, mBboxHead, bbox);
	}
	return false; 
}

bool Actor::DetectGrouped(AABB const& bbox) const
{
	if (AABB::DetectGroup(mBbox, bbox))
	{
		return	DetectPixel(*this, mBboxBody, bbox) ||
				DetectPixel(*this, mBboxHead, bbox);
	}
	return false;
}

bool Actor::DetectPixel(Actor const& other)
{
	return false;
}

bool Actor::DetectPixel(Actor const& other, AABB const& bboxOther) const
{
	if (AABB::Detect(mBbox, bboxOther))
	{
		return	DetectPixel(*this, other, mBboxBody, bboxOther) ||
				DetectPixel(*this, other, mBboxHead, bboxOther);
	}
	return false;
}

bool Actor::DetectPixel(AABB const& bbox) const
{
	return false;
}

bool Actor::DetectPixel(AreaInt const& overlap) const
{
	float2 const	topLeft		= mBbox.GetTopLeft();
	auto const&		spriteSheet = *mDisplay.GetSpriteSheet();
	uint8_t const*	pixels		= spriteSheet.pixels;

	int const x1 = overlap.mLeft, y1 = overlap.mTop;
	int const x2 = overlap.mRight, y2 = overlap.mBottom;
	int const w = x2 - x1, h = y2 - y1;

	int2 const dir = int2(x1, y1) - static_cast<int2>(topLeft);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int const addr = mDisplay.mFrame * spriteSheet.mFrameWidth + (dir.x + x) + (dir.y + y) * spriteSheet.width;

			uint8_t const c1 = spriteSheet.palette->indices[pixels[addr]]; 

			// if pixel is not transparent: 
			if (c1 != 0) return true;
		}
	}

	return false;
}

bool Actor::DetectPixel(Actor const& actor, AABB const& bboxActor, AABB const& bbox)
{
	if (AABB::Detect(bboxActor, bbox))  
	{
		Area const overlap = AABB::Overlap(bboxActor, bbox); 
		AreaInt const overlapI =
		{
			.mRight		= static_cast<int>(floor(overlap.mRight)),
			.mTop		= static_cast<int>(floor(overlap.mTop)),
			.mLeft		= static_cast<int>(floor(overlap.mLeft)),
			.mBottom	= static_cast<int>(floor(overlap.mBottom))
		};
		if (actor.DetectPixel(overlapI)) 
		{
			return true;
		}
	}
	return false;
}

bool Actor::DetectPixel(Actor const& actor1, Actor const& actor2, AABB const& bbox1, AABB const& bbox2)
{
	if (AABB::Detect(bbox1, bbox2))
	{
		Area const overlap = AABB::Overlap(bbox1, bbox2); 
		AreaInt const overlapI =
		{
			.mRight		= static_cast<int>(floor(overlap.mRight)),
			.mTop		= static_cast<int>(floor(overlap.mTop)),
			.mLeft		= static_cast<int>(floor(overlap.mLeft)),
			.mBottom	= static_cast<int>(floor(overlap.mBottom))
		};
		if (DetectPixel(actor1, actor2, overlapI))	
		{
			return true; 
		}
	}
	return false; 
}

bool Actor::DetectPixel(Actor const& actor1, Actor const& actor2, AreaInt const& overlap)
{
	float2 const   topLeft1		= actor1.GetBbox().GetTopLeft();
	float2 const   topLeft2		= actor2.GetBbox().GetTopLeft();
	auto const&    spriteSheet1	= *actor1.mDisplay.GetSpriteSheet();
	auto const&	   spriteSheet2	= *actor2.mDisplay.GetSpriteSheet();
	uint8_t const* pixels1		= spriteSheet1.pixels;
	uint8_t const* pixels2		= spriteSheet2.pixels;

	int const x1 = overlap.mLeft, y1 = overlap.mTop; 
	int const x2 = overlap.mRight, y2 = overlap.mBottom;  
	int const w = x2 - x1, h = y2 - y1;  

	int2 const dir1 = int2(x1, y1) - static_cast<int2>(topLeft1);
	int2 const dir2 = int2(x1, y1) - static_cast<int2>(topLeft2);

	int addr1, addr2; 
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			addr1 = actor1.mDisplay.mFrame * spriteSheet1.mFrameWidth + (dir1.x + x) + (dir1.y + y) * spriteSheet1.width;
			addr2 = actor2.mDisplay.mFrame * spriteSheet2.mFrameWidth + (dir2.x + x) + (dir2.y + y) * spriteSheet2.width;

			uint8_t const c1 = spriteSheet1.palette->indices[pixels1[addr1]];
			uint8_t const c2 = spriteSheet2.palette->indices[pixels2[addr2]];

			// if both pixels are not transparent:
			if (c1 != 0 && c2 != 0) return true;
		}
	}

	return false; 
}

void Actor::Move(float const dt)
{
	float const speed = mSpeed * dt; 
	if (!mBboxTile.DetectTilemap(mFacing, speed)) 
	{
		SetPosition(GetPosition() + cardinalToFloat2(mFacing) * speed); 
	}
}

void Actor::SetPosition(float2 const position)
{
	mPosF = position;
	mPosI = static_cast<int2>(mPosF);
	mBbox.Update();
	mBboxTile.Update();
	mBboxHead.Update();
	mBboxBody.Update(); 
}

void Actor::SetSpriteSheet(AnimationSpriteSheet& spriteSheet, int const dirAnimCount)
{
	mDirAnimCount	= dirAnimCount;
	mDisplay.SetSpriteSheet(&spriteSheet);
	mBbox.Resize({ spriteSheet.mFrameWidth, spriteSheet.height }, { -9, -25 });
	mBboxHead.Resize({ spriteSheet.mFrameWidth - 10, 8 }, { -4, -25 });  
	mBboxBody.Resize({ spriteSheet.mFrameWidth, spriteSheet.height - 3 }, { -9, -22 });
	mBboxTile.Resize({ spriteSheet.mFrameWidth - 4,9 }, { -7, -3 });
}

void Actor::SetAnimation() 
{ 
	int const animIdx = GetAnimationIdx();  
	if (animIdx != mDisplay.mAnimator.GetAnimIdx()) // not already playing the animation  
	{
		mDisplay.mAnimator.SetAnimation(animIdx);  
	}
}

int Actor::GetAnimationIdx() const  
{
	if (mAnimState + 1 > mDirAnimCount) 
	{
		// get whatever animation it is after the bundled animation states:
		return mDirAnimCount * 4 + (mAnimState - mDirAnimCount); 
	}
	// get the animation state and in the correct direction:
	return mFacing * mDirAnimCount + mAnimState; 
}

void Actor::SetBehavior(int const behavior)
{
	mBehavior = behavior;
	SetAnimationState();
	SetAnimation(); 
}

void Actor::SetFacing(int const facing)
{
	mFacing = facing; 
	SetAnimation();
}

void Actor::SetState(int const behavior, int const facing)
{
	mBehavior = behavior;
	mFacing = facing; 
	SetAnimationState();
	SetAnimation();
}

void Actor::SetPath(Sequence const* sequence)
{
	mSequencer.SetSequence(sequence);
}

void Actor::InitState()   
{
	SetState(0, EAST);  
}
