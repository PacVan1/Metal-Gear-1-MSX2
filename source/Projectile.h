#pragma once

#include "AABB.h" 
#include "ObjectPool.h" 

class Player;

class Projectile
{
private:
	AABB	mBbox; 
	float2	mSpeed;
	float2	mPosF;
	int		mDamage;

public:
			Projectile(); 
	void	Update(float const dt);
	void	Render(Surface8* screen);
	void	SetPosition(float2 const position);
	void	Launch(float2 const position, float2 const speed, int const damage, int const collisionGroup);

	inline float2		GetPosition() const { return mPosF; }
	inline AABB const&	GetBbox() const		{ return mBbox; }
	inline int			GetDamage() const	{ return mDamage; }
};

int constexpr PROJECTILE_COUNT = 100;

class Projectiles
{
private:
	ObjectPool<Projectile> mPool;  

public:
			Projectiles();
	void	Update(float const dt);
	void	Render(Surface8* screen);
	void	Launch(float2 const position, float2 const speed, int const damage, int const collisionGroup); 
};
