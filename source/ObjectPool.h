#pragma once

template<typename T>
class ObjectPool
{
public:
	uint const	SIZE;

private:
	T*			objects;
	bool*		active;
	uint		activeCount;

public:
	ObjectPool(size_t const size = 10) :
		SIZE(size),
		activeCount(0) 
	{
		objects	= new T[SIZE];
		active	= new bool[SIZE];
		memset(active, false, SIZE * sizeof(bool));
	}

	~ObjectPool()
	{
		delete[] objects;   
		delete[] active;   
	} 

	int WakeObject()
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (active[i]) continue;
			active[i] = true;
			activeCount++;
			return i;
		}
		return -1;
	}

	void ReturnObject(int const idx)
	{
		active[idx] = false;
		activeCount--; 
	}

	void ReturnAll()
	{
		for (int i = 0; i < SIZE; i++)
		{
			active[i] = false;
		}
		activeCount = 0; 
	}

	bool IsActive(int const idx) const { return active[idx]; }
	int GetActiveCount() const { return activeCount; }

	T& operator[](uint index)
	{
		return objects[index];
	}
	const T& operator[](uint index) const
	{
		return objects[index];
	}
};
