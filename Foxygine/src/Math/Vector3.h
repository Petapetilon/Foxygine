#pragma once
#include <math.h>
#include <glm.hpp>


class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(const float _x, const float _y, const float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3(const float _x, const float _y)
	{
		x = _x;
		y = _y;
		z = 0;
	}

	Vector3(const Vector3& source)
	{
		x = source.x;
		y = source.y;
		z = source.z;
	}

	Vector3(const glm::vec3& source) 
	{
		x = source.x;
		y = source.y;
		z = source.z;
	}

	Vector3& Normalized()
	{
		float mul = 1.f / sqrtf(x * x + y * y + z * z);
		*this = *this * mul;
		return *this;
	}

	static Vector3 Normalize(const Vector3& vec)
	{
		float mul = 1.f / sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		return Vector3(vec) * mul;
	}

	static float Magnitude(const Vector3& vec) { return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); }
	float Magnitude() { return sqrtf(x * x + y * y + z * z); }

	static float SqrMagnitude(const Vector3& vec) { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z; }
	float SqrMagnitude() { return x * x + y * y + z * z; }

	static float Distance(const Vector3& a, const Vector3& b) { return (a - b).Magnitude(); }
	float Distance(const Vector3& other) { return (*this - other).Magnitude(); }

	static float Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	float Dot(const Vector3& other) { return x * other.x + y * other.y + z * other.z; }

	static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	Vector3 Cross(const Vector3& other) {
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x);
	}

	float Angle(Vector3& a, Vector3& b)
	{
		float dot = Dot(a, b);
		dot *= dot;
		return acosf(sqrtf(dot / (a.SqrMagnitude() * b.SqrMagnitude())));
	}

	static Vector3 Project(const Vector3& vec, const Vector3& dir) { return Vector3(Vector3::Normalize(dir)) * Dot(vec, dir); }
	Vector3& Project(const Vector3& dir) { return *this = Vector3(Vector3::Normalize(dir)) * Dot(*this, dir); }

	static Vector3 ProjectOnPlane(const Vector3& vec, const Vector3& normal) { return Vector3(vec - Project(vec, normal)); }
	Vector3& ProjectOnPlane(Vector3& normal) { return *this = Vector3(*this - this->Project(normal)); }

	bool operator==(const Vector3& other) const
	{
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}
	// Bool Not Equals Operator Overload
	bool operator!=(const Vector3& other) const
	{
		return !(this->x == other.x && this->y == other.y && this->z == other.z);
	}
	// Addition Operator Overload
	Vector3 operator+(const Vector3& right) const
	{
		return Vector3(this->x + right.x, this->y + right.y, this->z + right.z);
	}
	// Subtraction Operator Overload
	Vector3 operator-(const Vector3& right) const
	{
		return Vector3(this->x - right.x, this->y - right.y, this->z - right.z);
	}
	// Float Multiplication Operator Overload
	Vector3 operator*(const float& other) const
	{
		return Vector3(this->x * other, this->y * other, this->z * other);
	}
	// Float Division Operator Overload
	Vector3 operator/(const float& other) const
	{
		return Vector3(this->x / other, this->y / other, this->z / other);
	}

	operator glm::vec3() const { return glm::vec3(x, y, z); }
};

