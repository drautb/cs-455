#pragma once

#include <ostream>

#include <Eigen\Core>
#include <Eigen\LU>

#include "cs455Utils.h"

using namespace std;
using namespace Eigen;

class Matrix455 : public Matrix4f
{
public:
	const static int ROWS = 4;
	const static int COLS = 4;

	Matrix455(void):Matrix4f() {}
	typedef Matrix4f Base;

	// This constructor allows you to construct Matrix455 from Eigen expressions
	template<typename OtherDerived>
	Matrix455(const Eigen::MatrixBase<OtherDerived>& other) : Eigen::Matrix4f(other)
	{
		this->Base(other);
	}

	// This method allows you to assign Eigen expressions to MyVectorType
	template<typename OtherDerived>
	Matrix455 & operator= (const Eigen::MatrixBase <OtherDerived>& other)
	{
		this->Base::operator=(other);
		return *this;
	}

	/**
	 * This method puts an array of 16 values into the matrix in column major format
	 */
	Matrix455 &operator=(float *values)
	{
		for (int i=0; i<COLS; i++)
		{
			for (int j=0; j<ROWS; j++)
			{
				this->col(i)(j) = values[i * COLS + j];
			}
		}

		return *this;
	}

	const float *toArray()const
	{
		return this->data();
	}

	void toArray(float *array)
	{
		for (int i=0; i<ROWS*COLS; i++)
			array[i] = this->data()[i];
	}

	/**
	 * TEST METHOD
	 */
	static bool Test(ostream &output)
	{
		bool success = true;

		output << endl << "Testing Matrix455...   " << endl;

		output << "1) Read/Write to simple array of floats in column major format...  ";

		Matrix455 mat;
		mat = Matrix455::Zero();
		float array[ROWS * COLS] = { 1, 0, 0, 0, 2, 5, 0, 0, 3, 6, 8, 0, 4, 7, 9, 0 };
		mat = array;

		// Verify Assignment
		int arrayIndex = 0;
		for (int c=0; c<COLS; c++)
		{
			for (int r=0; r<ROWS; r++)
			{
				TEST(mat(r, c) == array[arrayIndex++]);					
			}
		}

		// Go the other way
		float recvArray[ROWS * COLS];
		mat.toArray(recvArray);

		// Verify new array
		for (int i=0; i<ROWS*COLS; i++)
			TEST(recvArray[i] == array[i]);

		if (success)
			output << "\tSucceeded" << endl;
		else
			return success;

		output << "2) Multiply Matrices by eachother...  ";

		Matrix455 mat2;
		float array2[ROWS * COLS] = { 1, 0, 0, 0, 0, 2, 0, 0, 1, 0, 3, 0, 0, 2, 0, 4 };
		mat2 = array2;

		Matrix455 result;
		result = mat * mat2;

		// Verify matrix result
		Matrix455 expectedResult;
		expectedResult = Matrix455::Zero();
		float arrayResult[ROWS * COLS] = { 1, 0, 0, 0, 4, 10, 0, 0, 10, 18, 24, 0, 20, 38, 36, 0 };
		expectedResult = arrayResult;
		TEST(result == expectedResult);

		if (success)
			output << "\t\t\t\t\tSucceeded" << endl;
		else
			return success;

		output << "3) Perform Matrix Inversion...  ";

		Matrix455 expectedInverse;
		float arrayInv[ROWS * COLS] = { 1, -0, 0, -0, -0, 0.5f, -0, 0, -0.333333f, -0, 0.333333f, -0, -0, -0.25f, -0, 0.25f };
		expectedInverse = arrayInv;
		TEST(expectedInverse.isApprox(mat2.inverse()));

		if (success)
			output << "\t\t\t\t\tSucceeded" << endl;
		else
			return success;

		output << "4) Multiply a matrix by a vector...  ";

		Vector455 vec;
		float vecVals[ROWS] = { 2, 3, 5, 7 };
		vec = vecVals;

		Vector455 resultVec, expectedVecResult;
		float expVecVals[ROWS] = { 51, 94, 103, 0 };
		expectedVecResult = expVecVals;
		resultVec = mat * vec;
		TEST(resultVec.isApprox(expectedVecResult));

		if (success)
			output << "\t\t\t\t\tSucceeded" << endl;
		else
			return success;

		return success;
	}
};

