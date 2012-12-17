#pragma once

#include <iostream>
#include <ostream>

#include "Matrix4.h"

using namespace std;

const int VECTOR_SIZE = 4;

template <typename T>
class Vector4
{
private:
	/**
	 * Values is an array of 4 of T, representing a vector. 
	 * An array like this: { 3, 5, 7, 9 } corresponds to 
	 * a vector like this:
	 *
	 *		[ 3 ]
	 *		[ 5 ]
	 *		[ 7 ]
	 *		[ 9 ]
	 */
	T *values;

	/**
	 * Class constants
	 */
	static const int OutOfRangeException = 1;

public:
	/**
	 * Constructors
	 */
	Vector4()
	{
		values = NULL;
		AllocateMemory();
	}

	Vector4(T initialValue)
	{
		values = NULL;
		AllocateMemory();

		for (int i=0; i<VECTOR_SIZE; i++)
			values[i] = initialValue;
	}

	Vector4(const Vector4 &other)
	{
		values = NULL;
		AllocateMemory();

		CopyValues(other.values);
	}

	/**
	 * Destructor
	 */
	~Vector4()
	{
		FreeMemory();
	}

	/**
	 * Operator Overloads
	 */
	Vector4 &Vector4::operator=(const Vector4 &rhs)
	{
		CopyValues(rhs.values);
		return *this;
	}

	bool Vector4::operator==(const Vector4 &rhs)
	{
		return AllValuesEqual(rhs.values);
	}

	bool Vector4::operator!=(const Vector4 &rhs)
	{
		return !AllValuesEqual(rhs.values);
	}

	T &Vector4::operator()(int i)
	{
		if (i < 0 || i >= VECTOR_SIZE)
			throw OutOfRangeException;

		return values[i];
	}

	Vector4 &Vector4::operator*=(const Vector4 &rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	Vector4 &Vector4::operator*=(const int &rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	Vector4 &Vector4::operator*=(const float &rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	Vector4 &Vector4::operator*=(const double &rhs)
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

	T Get(int i)const
	{
		return values[i];
	}

private:
	void AllocateMemory()
	{
		FreeMemory();
		values = new T[VECTOR_SIZE];
		memset(values, 0, sizeof(T) * VECTOR_SIZE);
	}

	void FreeMemory()
	{
		if (values != NULL)
			delete[] values;

		values = NULL;
	}

	void CopyValues(T *otherValues)
	{
		for (int i=0; i<VECTOR_SIZE; i++)
			values[i] = otherValues[i];
	}

	bool AllValuesEqual(T *otherValues)
	{
		for (int i=0; i<VECTOR_SIZE; i++)
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
		cout << endl << "TESTING VECTOR4...." << endl;
	}
};

/********************************************************************
 * Other Operator Overloads
 */
template <typename T>
Vector4<T> operator*(const Vector4<T> &lhs, const Vector4<T> &rhs)
{
	Vector4<T> result;

	return result;
}

template <typename T>
Vector4<T> operator*(const Matrix4<T> &lhs, const Vector4<T> &rhs)
{
	Vector4<T> result;

	return result;
}

template <typename T>
Vector4<T> operator*(const Vector4<T> &lhs, const int &scalar)
{
	Vector4<T> result = lhs;

	for (int i=0; i<VECTOR_SIZE; i++)
		result(i) *= scalar;

	return result;
}

template <typename T>
Vector4<T> operator*(const Vector4<T> &lhs, const float &scalar)
{
	Vector4<T> result = lhs;

	for (int i=0; i<VECTOR_SIZE; i++)
		result(i) *= scalar;

	return result;
}

template <typename T>
Vector4<T> operator*(const Vector4<T> &lhs, const double &scalar)
{
	Vector4<T> result = lhs;

	for (int i=0; i<VECTOR_SIZE; i++)
		result(i) *= scalar;

	return result;
}

template <typename T>
ostream &operator<<(ostream &out, Vector4<T> &rhs)
{
	out << endl;
	
	for (int i=0; i<VECTOR_SIZE; i++)
		out << "[ " << values[i] << "\t]" << endl;

	return out;
}


