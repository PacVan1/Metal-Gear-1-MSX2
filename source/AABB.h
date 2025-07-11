#pragma once

uint8_t constexpr BBOX_HIT_COLOR	= 181;
uint8_t constexpr BBOX_MISS_COLOR	= 140; 

struct TileArea; 

enum collisionGroups : uint8_t 
{
	COLLISION_GROUPS_NONE,
	COLLISION_GROUPS_PLAYER,
	COLLISION_GROUPS_ENEMY,
	COLLISION_GROUPS_COUNT, 
};

struct Area final
{
	float mRight	= 0.0f;
	float mTop		= 0.0f;
	float mLeft		= 0.0f;
	float mBottom	= 0.0f;
};

struct AreaInt final
{
	int mRight	= 0;
	int mTop	= 0;
	int mLeft	= 0;
	int mBottom = 0;
};

class AABB final 
{
public:
	int				mGroup;
#if DEBUG_MODE
	bool			mHit = false;
#endif

private:
	float2 const*	mPos; 
	float2			mHalfSize;        
	float2			mTopLeft; 
	float2			mBottomRight; 
	float2			mOffset;   
	int2 			mSize;

public:
	static bool Detect(AABB const& bbox1, AABB const& bbox2);
	static bool DetectGroup(AABB const& bbox1, AABB const& bbox2); 
	static bool Detect(AABB const& bbox, float const right, float const top, float const left, float const bottom);
	static bool Detect(float2 const point, float const right, float const top, float const left, float const bottom);
	static bool Detect(int2 const point, int const right, int const top, int const left, int const bottom);
	static Area Overlap(AABB const& bbox1, AABB const& bbox2);
	static bool DetectTilemap(TileArea const& area);  

public:
					AABB() = default;
					AABB(float2 const* position); 
					AABB(float2 const* position, int2 const size, float2 const offset);
	void			Update();  
	void			Render(Surface8* screen, uint8_t const cMiss = BBOX_MISS_COLOR, uint8_t const cHit = BBOX_HIT_COLOR) const; 
	void			Resize(int2 const size, int2 const offset); 
#if DEBUG_MODE
	bool			Detect(AABB& other); 
#else
	bool			Detect(AABB const& other) const; 
#endif
	bool			Detect(float2 const point) const;
	bool			DetectTilemap() const; 
	bool			DetectTilemap(int cardinal, float const distance) const;

	inline float2	GetPosition() const		{ return *mPos; }
	inline float2	GetHalfSize() const		{ return mHalfSize; }
	inline float2	GetTopLeft() const		{ return mTopLeft; }
	inline float2	GetBottomRight() const	{ return mBottomRight; }
	inline float2	GetOffset() const		{ return mOffset; }
	inline int2		GetSize() const			{ return mSize; }
};

