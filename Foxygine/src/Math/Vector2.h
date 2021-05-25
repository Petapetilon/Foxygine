#pragma once
#include "glm.hpp"
#include <string>


class Vector2 {
public:
	float x;
	float y;

	Vector2() { x = y = 0; }

	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}


	static Vector2 Rotate(Vector2 vec, float angle) {
		float rad = glm::radians(angle);
		float s = sin(rad);
		float c = cos(rad);
		float oldX = vec.x;
		vec.x = vec.x * c - vec.y * s;
		vec.y = oldX * s + vec.y * c;
		return vec;
	}


	Vector2 Rotated(float angle) {
		Vector2 vec;
		float rad = glm::radians(angle);
		float s = sin(rad);
		float c = cos(rad);
		float oldX = x;
		vec.x = x * c - y * s;
		vec.y = oldX * s + y * c;
		return vec;
	}


	static Vector2 Unsigne(Vector2 vec) { return Vector2(abs(vec.x), abs(vec.y)); }
	Vector2 Unsigned() { return Vector2(abs(x), abs(y)); }

	static Vector2 Normalize(Vector2 vec) { return vec / sqrtf(vec.x * vec.x + vec.y * vec.y); }
	Vector2 Normalized() { return Vector2(x, y) / sqrtf(x * x + y * y); }

	static float Magnitude(const Vector2& vec) { return sqrtf(vec.x * vec.x + vec.y * vec.y); }
	float Magnitude() { return sqrtf(x * x + y * y); }

	static float SqrMagnitude(const Vector2& vec) { return vec.x * vec.x + vec.y * vec.y; }
	float SqrMagnitude() { return x * x + y * y; }

	static float Distance(const Vector2& a, const Vector2& b) { return (a - b).Magnitude(); }
	float Distance(const Vector2& other) { return (*this - other).Magnitude(); }

	static float Dot(const Vector2& a, const Vector2& b) { return a.x * b.x + a.y * b.y; }
	float Dot(const Vector2& other) { return x * other.x + y; }

	Vector2 ComponentMul(Vector2 other) { return Vector2(x * other.x, y * other.y); }
	Vector2 ComponentDiv(Vector2 other) { return Vector2(x / other.x, y / other.y); }


	std::string Debug() { return "x: " + std::to_string(x) + ", y: " + std::to_string(y); }


	bool operator==(const Vector2& other) const
	{
		return (this->x == other.x && this->y == other.y);
	}
	// Bool Not Equals Operator Overload
	bool operator!=(const Vector2& other) const
	{
		return !(this->x == other.x && this->y == other.y);
	}
	// Addition Operator Overload
	Vector2 operator+(const Vector2& right) const
	{
		return Vector2(this->x + right.x, this->y + right.y);
	}
	// Subtraction Operator Overload
	Vector2 operator-(const Vector2& right) const
	{
		return Vector2(this->x - right.x, this->y - right.y);
	}
	// Float Multiplication Operator Overload
	Vector2 operator*(const float& other) const
	{
		return Vector2(this->x * other, this->y * other);
	}
	// Float Division Operator Overload
	Vector2 operator/(const float& other) const
	{
		return Vector2(this->x / other, this->y / other);
	}

	operator glm::vec2() const { return glm::vec2(x, y); }
	operator glm::vec3() const { return glm::vec3(x, y, 0); }
};