#pragma once

#include <Eigen/Core>

#include "Matrix455.h"

#define ROWS		4

using namespace Eigen;

class Vector455 : public Vector4f
{
public:
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

	const float *toArray()const
	{
		return this->data();
	}
};

