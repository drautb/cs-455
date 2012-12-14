#pragma once

#include <ostream>

#include "Vector4.h"

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
	static const int MATRIX_ROW_SIZE = 4;
	static const int MATRIX_COL_SIZE = 4;
	static const int MATRIX_SIZE = MATRIX_ROW_SIZE * MATRIX_COL_SIZE;

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

	Matrix4 &Matrix4::operator=(const T *newValues)
	{
		CopyValues(newValues);
		return *this;
	}

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
		if (x < 0 || x >= MATRIX_ROW_SIZE || y < 0 || y >= MATRIX_COL_SIZE)
			throw OutOfRangeException;

		return values[x * MATRIX_COL_SIZE + y];
	}

	Matrix4 Matrix4::operator*(const Matrix4 &lhs, const Matrix4 &rhs)
	{
		Matrix4 result;

		// For each row in lhs
			// multiply each entry by each entry in each column in rhs
			// the result goes in row of lhs col of rhs position in the final

		return result;
	}

	Matrix4 &Matrix4::operator*=(const Matrix4 &rhs)
	{
		this = this * rhs;

		return *this;
	}

	ostream &operator<<(ostream &out, Matrix4 &rhs)
	{
		for (int row=0; row<MATRIX_ROW_SIZE; row++)
		{
			out << "[\t";

			for (int col=0; col<MATRIX_COL_SIZE; col++)
				out << rhs(col, row) << "\t";

			out << "]\n";
		}

		return out;
	}

	/**
	 * Other Methods
	 */
	const T *ToArray()
	{
		return values;
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
};

