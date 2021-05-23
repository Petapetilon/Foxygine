#pragma once
#include "Vector2.h"


class Vector2I {
public:
	int x;
	int y;

	Vector2I() {
		x = 0;
		y = 0;
	}

	Vector2I(int _x, int _y) {
		x = _x;
		y = _y;
	}

	operator Vector2() const { return Vector2(x, y); }
};