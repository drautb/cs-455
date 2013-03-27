#pragma once

#include <ostream>

#include <GL/glfw.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "cs455Utils.h"

using namespace std;
using namespace Eigen;

class Vector455 : public Vector4f
{
public:
	const static int ROWS = 4;

	Vector455(void):Vector4f() {}
	typedef Vector4f Base;
	
	// This constructor allows you to construct MyVectorType from Eigen expressions
	template<typename OtherDerived>
	Vector455(const MatrixBase<OtherDerived>& other) : Vector4f(other)
	{ 
	
	}
	
	// This method allows you to assign Eigen expressions to MyVectorType
	template<typename OtherDerived>
	Vector455 & operator= (const Eigen::MatrixBase <OtherDerived>& other)
	{
		this->Base::operator=(other);
		return *this;
	}

	/**
	 * This method puts an array of 4 values into the vector
	 */
	Vector455 &operator=(float *values)
	{
		for (int j=0; j<ROWS; j++)
		{
			this->row(j)(0) = values[j];
		}

		return *this;
	}

	Vector455 &operator=(const GLfloat *values)
	{
		for (int j=0; j<ROWS; j++)
		{
			this->row(j)(0) = values[j];
		}

		return *this;
	}

	void toArray(float *array)
	{
		for (int i=0; i<ROWS; i++)
			array[i] = this->data()[i];
	}

	const float *toArray()const
	{
		return this->data();
	}

	float &r()
	{
		return this->row(0)(0);
	}

	float &g()
	{
		return this->row(1)(0);
	}

	float &b()
	{
		return this->row(2)(0);
	}

	float &a()
	{
		return this->row(3)(0);
	}

	/**
	 * TEST METHOD
	 */
	static bool Test(ostream &output)
	{
		bool success = true;

		output << endl << "Testing Vector455...   " << endl;

		output << "1) Read/Write to simple array of floats in column major format...  ";

		Vector455 vec;
		vec = vec.Zero();
		float array[ROWS] = { 2, 3, 5, 7 };
		vec = array;

		// Verify Assignment
		for (int i=0; i<ROWS; i++)
			TEST(vec(i) == array[i]);

		// Go the other way
		float recvArray[ROWS];
		vec.toArray(recvArray);

		for (int i=0; i<ROWS; i++)
			TEST(recvArray[i] == array[i]);

		if (success)
			output << "\tSucceeded" << endl;
		else
			return success;

		output << "2) Multiply vectors by single numbers...  ";

		// Scalar Multiplication
		Vector455 resultVec;
		resultVec = vec * 2;
		Vector455 expectedResultVec;
		float expMultRes[ROWS] = { 4, 6, 10, 14 };
		expectedResultVec = expMultRes;
		TEST(resultVec == expectedResultVec);

		if (success)
			output << "\t\t\t\tSucceeded" << endl;
		else
			return success;

		output << "3) Add/Subtract vectors...  ";

		// Add/Subtract Vectors
		Vector455 addVector;
		float newVals[ROWS] = { 12, 34, -23, 43 };
		addVector = newVals;
		expectedResultVec = vec;
		vec = vec + addVector;
		vec = vec - addVector;
		TEST(vec == expectedResultVec);

		if (success)
			output << "\t\t\t\t\t\tSucceeded" << endl;
		else
			return success;

		output << "4) Take the inner or dot product of two vectors...  ";

		float vec1Vals[ROWS] = { 2, 4, 6, 0 };
		float vec2Vals[ROWS] = { 3, 6, 9, 0 };
		vec = vec1Vals;
		Vector455 vec2;
		vec2 = vec2Vals;
		TEST(vec.dot(vec2) == 84);

		if (success)
			output << "\t\t\tSucceeded" << endl;
		else
			return success;

		output << "5) Take the 3D cross product of two vectors...  ";

		float vec3Vals[ROWS] = { 1, 10, 30, 0 };
		float vec4Vals[ROWS] = { 20, 4, 50, 0 };
		float resultVals[ROWS] = { 380, 550, -196, 0 };
		vec = vec3Vals;
		vec2 = vec4Vals;
		expectedResultVec = resultVals;
		TEST(vec.cross3(vec2).isApprox(expectedResultVec));

		if (success)
			output << "\t\t\tSucceeded" << endl;
		else
			return success;

		return success;
	}
};

