#pragma once

#include <Eigen\Core>

#define ROWS		4
#define COLS		4

using namespace Eigen;

class Matrix455 : public Matrix4f
{
public:
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
};

