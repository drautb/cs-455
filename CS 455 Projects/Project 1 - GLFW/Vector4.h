#pragma once

class Vector4
{
public:
	float values[4];

public:
	Vector4(void);
	~Vector4(void);

	float DotProduct(Vector4 *other);
	void CrossProduct(Vector4 *other, Vector4 *result);
};

