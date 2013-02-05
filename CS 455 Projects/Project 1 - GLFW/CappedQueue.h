#pragma once

#include <queue>

using namespace std;

template <typename T>
class CappedQueue
{
private:

	const static int MAX_SIZE			= 10;
	const static int DEFAULT_CAP		= 3;

	T									q[MAX_SIZE];

	int									sizeCap;

	int									currentIdx;
	int									size;

public:

	CappedQueue(void)
	{
		SetCap(DEFAULT_CAP);
	}

	CappedQueue(int startingCap)
	{
		SetCap(startingCap);
	}

	~CappedQueue(void)
	{
		
	}

	void Push(const T& item)
	{
		currentIdx = nextIndex();
		q[currentIdx] = item;
		size++;

		cap();
	}

	T PopFront()
	{
		currentIdx = prevIndex();
		size--;

		return q[nextIndex()];
	}

	T PopBack()
	{
		int backIdx = backIndex();

		size--;

		return q[backIdx];
	}

	T Front()
	{
		return q[currentIdx];
	}

	T Back()
	{
		return q[backIndex()];
	}

	int SetCap(int cap)
	{
		if (cap > 0 && cap < MAX_SIZE)
			sizeCap = cap;
		else
			sizeCap = DEFAULT_CAP;

		Clear();

		return sizeCap;
	}

	void Clear()
	{
		size = 0;
		currentIdx = 0;
	}

	int Size()
	{
		return size;
	}

private:

	void cap()
	{
		while (size > sizeCap)
			PopFront();
	}

	int nextIndex()
	{
		if (currentIdx + 1 > sizeCap - 1)
			return 0;

		return currentIdx + 1;
	}

	int prevIndex()
	{
		if (currentIdx - 1 < 0)
			return sizeCap - 1;

		return currentIdx - 1;
	}

	int backIndex()
	{
		int backIdx = currentIdx;
		for (int i=size-1; i>0; i--)
		{
			backIdx--;
			if (backIdx < 0)
				backIdx = sizeCap - 1;
		}

		return backIdx;
	}
};

