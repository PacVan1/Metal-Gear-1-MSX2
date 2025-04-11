#pragma once

enum wakeTypes : int8_t
{
	WAKE_TYPES_FULL = -1
};

template<typename T>
class ObjectPool
{
public:
	int const	SIZE;

private:
	T*			objects;
	bool*		active;
	int*		activeIdxs;
	int			activeCount;

public:
	ObjectPool(int const size = 10) :
		SIZE(size),
		activeCount(0) 
	{
		objects		= new T[SIZE];
		active		= new bool[SIZE];
		activeIdxs	= new int[SIZE];
		memset(active, false, SIZE * sizeof(bool));
		memset(activeIdxs, 0, SIZE * sizeof(bool));
	}

	~ObjectPool()
	{
		delete[] objects;   
		delete[] active;   
	} 

	int WakeObject()
	{
		if (activeCount == SIZE) return WAKE_TYPES_FULL;

		for (int i = 0; i < SIZE; i++)
		{
			if (active[i]) continue;
			active[i] = true;
			activeIdxs[activeCount++] = i;
 			return i;
		}
		return WAKE_TYPES_FULL;
	}

	void ReturnObject(int const idx)
	{
		active[idx] = false;
		activeCount--; 
	}

	void ReturnActive(int const idx)
	{
		active[activeIdxs[idx]] = false;
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

	T& GetActive(int const idx)
	{
		return objects[activeIdxs[idx]];
	}

	T const& GetActive(int const idx) const
	{
		return objects[activeIdxs[idx]];
	}

	inline bool IsActive(int const idx) const { return active[idx]; }
	inline int GetActiveCount() const { return activeCount; }

	T& operator[](int index) 
	{
		return objects[index];
	}
	T const& operator[](int index) const 
	{
		return objects[index];
	}
};
