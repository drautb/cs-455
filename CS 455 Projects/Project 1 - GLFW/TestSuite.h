#pragma once

#include <ostream>

#include "cs455Utils.h"
#include "Matrix455.h"
#include "Vector455.h"

using namespace std;

class TestSuite
{
public:

	/**
	 * Constructors/Destructors never used, this class should
	 * never be instantiated.
	 */
	TestSuite(void) {}
	~TestSuite(void) {}

	/**
	 * Begin Static Test Methods
	 */
	static bool RunAll(ostream &output)
	{
		bool success = true;

		output << endl << "\t*** RUNNING ALL TESTS ***" << endl;

		if (!TestP1Math(output)) success = false;
		if (!TestP1Stack(output)) success = false;

		if (success)
			output << endl << "\t*** ALL TESTS SUCCEEDED ***";
		else
			output << endl << "TESTS FAILED!";

		return success;
	}

	static bool TestP1Math(ostream &output)
	{
		bool success = true;

		output << endl << "\t*** TestP1Math() ***";

		if (!Matrix455::Test(output)) success = false;
		if (!Vector455::Test(output)) success = false;

		return success;
	}

	static bool TestP1Stack(ostream &output)
	{
		bool success = true;

		output << endl << "\t*** TestP1Stack() ***";

		const int stackSize = 512;
		MAT_STACK_455 matStack;

		output << endl << "1) Pushing " << stackSize << " matrices onto the stack...  ";

		Matrix455 matrices[stackSize];

		for (int i=0; i<stackSize; i++)
		{
			matrices[i] = Matrix455::Random();
			matStack.push(matrices[i]);
		}

		TEST(matStack.size() == stackSize);

		if (success)
			output << "\t\t\t\tSucceeded";
		else
			return success;

		output << endl << "2) Popping " << stackSize << " matrices off stack... ";

		for (int i=0; i<stackSize; i++)
			matStack.pop();

		TEST(matStack.size() == 0);

		if (success)
			output << "\t\t\t\t\tSucceeded" << endl;
		else
			return success;

		return success;
	}
};

