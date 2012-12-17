#pragma once

#include <iostream>
#include <ostream>

#include "Vector4.h"

using namespace std;

const int MATRIX_ROW_SIZE = 4;
const int MATRIX_COL_SIZE = 4;
const int MATRIX_SIZE = MATRIX_ROW_SIZE * MATRIX_COL_SIZE;

template <typename T>
class Matrix4
{
private:
	/**
	 * Values in this 4x4 matrix are stored in a one dimeonsional array, in column-major
	 * format. This means that the following matrix:
	 *
	 *		[  1,  2,  3,  4 ]
	 *		[  5,  6,  7,  8 ]
	 *		[  9, 10, 11, 12 ]
	 *		[ 13, 14, 15, 16 ]
	 *
	 * Would be stored in the 1-D array values like this:
	 *
	 *		{ 1, 5, 9, 13, 2, 6, 10, 14, 3 .... etc}
	 *
	 * This is require by the specs for CS455, because it is the format OpenGL uses.
	 * 
	 */
	T *values;

	/**
	 * Class constants
	 */
	static const int OutOfRangeException = 1;

public:
	/**
	 * Various Constructors
	 */
	Matrix4()
	{
		values = NULL;
		AllocateMemory();
	}

	Matrix4(T initialValue)
	{
		values = NULL;
		AllocateMemory();

		for (int i=0; i<MATRIX_SIZE; i++)
			values[i] = initialValue;
	}

	Matrix4(const Matrix4 &other)
	{
		values = NULL;
		AllocateMemory();

		CopyValues(other.values);
	}

	/**
	 * Destructor
	 */
	~Matrix4()
	{
		FreeMemory();
	}

	/**
	 * Operator Overloads
	 */
	Matrix4 &Matrix4::operator=(const Matrix4 &rhs)
	{
		CopyValues(rhs.values);
		return *this;
	}

	/*Matrix4 &Matrix4::operator=(const T *newValues)
	{
		CopyValues(newValues);
		return *this;
	}*/
	
	bool Matrix4::operator==(const Matrix4 &rhs)
	{
		return AllValuesEqual(rhs.values);
	}

	bool Matrix4::operator!=(const Matrix4 &rhs)
	{
		return !AllValuesEqual(rhs.values);
	}

	// x and y are 0 indexed, i.e. DON'T TRY TO REFERENCE 4,4!
	T &Matrix4::operator()(int x, int y)
	{
		return values[GetIndex(x, y)];
	}

	Matrix4 &Matrix4::operator*=(const Matrix4 &rhs)
	{
		*this = *this * rhs;

		return *this;
	}

	/**
	 * Other Methods
	 */
	const T *ToArray()
	{
		return values;
	}

	void SetValues(T *newValues)
	{
		CopyValues(newValues);
	}

	T Get(int x, int y)const
	{
		return values[GetIndex(x, y)];
	}

	Matrix4 Inverted()const
	{
		return *this;
	}

private:
	void AllocateMemory()
	{
		FreeMemory();
		values = new T[MATRIX_SIZE];
		memset(values, 0, sizeof(T) * MATRIX_SIZE);
	}

	void FreeMemory()
	{
		if (values != NULL)
			delete[] values;

		values = NULL;
	}

	int GetIndex(int x, int y)const
	{
		if (x < 0 || x >= MATRIX_ROW_SIZE || y < 0 || y >= MATRIX_COL_SIZE)
			throw OutOfRangeException;

		return (x * MATRIX_COL_SIZE + y);
	}

	void CopyValues(T *otherValues)
	{
		for (int i=0; i<MATRIX_SIZE; i++)
			values[i] = otherValues[i];
	}

	bool AllValuesEqual(T *otherValues)
	{
		for (int i=0; i<MATRIX_SIZE; i++)
		{
			if (values[i] != otherValues[i])
				return false;
		}
		
		return true;
	}

public:
	/**
	 * CODE FOR TESTING
	 */
	static void Test()
	{
		Matrix4<float> mat;

		cout << "Created a matrix:" << mat << endl;
		cout << "Assigning values..." << endl;
		float vals[MATRIX_SIZE] = { 1.0f, 2.0f, 3.0f, 4.0f, 
									5.0f, 6.0f, 7.0f, 8.0f, 
									9.0f, 10.0f, 11.0f, 12.0f, 
									13.0f, 14.0f, 15.0f, 16.0f };
		mat.SetValues(&vals[0]);
		cout << mat << endl;
		cout << "Matrix[2,3] = " << mat(2,3) << endl;

		Matrix4<float> newMat;
		float newVals[MATRIX_SIZE] = { 1.0f, 0.0f, 0.0f, 0.0f,
									   2.0f, 5.0f, 0.0f, 0.0f,
									   3.0f, 6.0f, 8.0f, 0.0f,
									   4.0f, 7.0f, 9.0f, 0.0f };
		newMat.SetValues(newVals);

		cout << "New Matrix: " << newMat << endl;
		cout << "Assigning newMat to mat..." << endl;
		mat = newMat;
		cout << "Now Mat is: " << mat << endl;
		//cout << "Resetting newVals... " << endl;

		float otherVals[MATRIX_SIZE] = { 1.0f, 0.0f, 0.0f, 0.0f,
										 0.0f, 2.0f, 0.0f, 0.0f,
										 1.0f, 0.0f, 3.0f, 0.0f,
										 0.0f, 2.0f, 0.0f, 4.0f };
		newMat.SetValues(otherVals);
		cout << "NewMat is now: " << newMat << endl;
		cout << "Multiplying mat * newMat...result: " << endl;
		mat *= newMat;
		cout << mat << endl;
	}
};

/********************************************************************
 * Other Operator Overloads
 */

template <typename T>
Matrix4<T> operator*(const Matrix4<T> &lhs, const Matrix4<T> &rhs)
{
	Matrix4<T> result;
	
	for (int row=0; row<MATRIX_ROW_SIZE; row++)
	{
		for (int col=0; col<MATRIX_COL_SIZE; col++)
		{
			T sum = lhs.Get(0, row) * rhs.Get(col, 0) +
					lhs.Get(1, row) * rhs.Get(col, 1) +
					lhs.Get(2, row) * rhs.Get(col, 2) +
					lhs.Get(3, row) * rhs.Get(col, 3);

			result(col, row) = sum;
		}
	}

	return result;
}

template <typename T>
ostream &operator<<(ostream &out, Matrix4<T> &rhs)
{
	out << endl;
	for (int row=0; row<MATRIX_ROW_SIZE; row++)
	{
		out << "[\t";

		for (int col=0; col<MATRIX_COL_SIZE; col++)
			out << rhs(col, row) << "\t";

		out << "]\n";
	}

	return out;
}
