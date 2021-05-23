#pragma once
#include "Vector3.h"


class Vector4 {
public:
	float x;
	float y;
	float z;
	float w;

	Vector4() { x = y = z = w = 0; }

	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vector4(glm::vec3 xyz, float _w) {
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
		w = _w;
	}

	bool operator==(const Vector4& other) const
	{
		return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
	}
	// Bool Not Equals Operator Overload
	bool operator!=(const Vector4& other) const
	{
		return !(this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
	}
	// Addition Operator Overload
	Vector4 operator+(const Vector4& right) const
	{
		return Vector4(this->x + right.x, this->y + right.y, this->z + right.z, this->w + right.w);
	}
	// Subtraction Operator Overload
	Vector4 operator-(const Vector4& right) const
	{
		return Vector4(this->x - right.x, this->y - right.y, this->z - right.z, this->w - right.w);
	}
	// Float Multiplication Operator Overload
	Vector4 operator*(const float& other) const
	{
		return Vector4(this->x * other, this->y * other, this->z * other, this->w * other);
	}
	// Float Division Operator Overload
	Vector4 operator/(const float& other) const
	{
		return Vector4(this->x / other, this->y / other, this->z / other, this->w / other);
	}

	operator glm::vec4() const { return glm::vec4(x, y, z, w); }
	operator Vector3() const { return Vector3(x, y, z); }
};