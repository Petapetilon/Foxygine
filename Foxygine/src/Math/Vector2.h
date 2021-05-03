#pragma once
#include "glm.hpp"


class Vector2 {
public:
	float x;
	float y;

	Vector2() { x = y = 0; }

	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}


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
};