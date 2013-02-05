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
		setCap(DEFAULT_CAP);
	}

	CappedQueue(int startingCap)
	{
		setCap(startingCap);
	}

	~CappedQueue(void)
	{
		
	}

	void push(const T& item)
	{
		currentIdx = nextIndex();
		q[currentIdx] = item;
		size++;

		cap();
	}

	T pop()
	{
		currentIdx = prevIndex();
		size--;

		return q[nextIndex()];
	}

	int setCap(int cap)
	{
		if (cap > 0 && cap < MAX_SIZE)
			sizeCap = cap;
		else
			sizeCap = DEFAULT_CAP;

		clear();

		return sizeCap;
	}

	void clear()
	{
		size = 0;
		currentIdx = 0;
	}

private:

	void cap()
	{
		while (size > sizeCap)
			pop();
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
};

