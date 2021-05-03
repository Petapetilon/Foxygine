#pragma once
#include <math.h>


class Color {
public:
	float a;
	float r;
	float g;
	float b;

	Color() { a = r = g = b = 1; }

	Color(float _r, float _g, float _b, float _a) {
		a = _a;
		r = _r;
		g = _g;
		b = _b;
	}

	Color(char _r, char _g, char _b, char _a) {
		a = (float)_a / 256.f;
		r = (float)_r / 256.f;
		g = (float)_g / 256.f;
		b = (float)_b / 256.f;
	}

	Color(int hue, float saturation, float value) {
		a = r = g = b = 1;
		if (saturation == 0) {
			a = 1;
			r = g = b = value;
		}
	
		hue %= 360;
		hue /= 60;
		int i = (int)floor(hue);
		auto f = hue - i;
		auto p = value * (1 - saturation);
		auto q = value * (1 - (saturation * f));
		auto t = value * (1 - (saturation * (1 - f)));

		switch (i) {
		case 0:
			r = value, g = t, b = p;
			break;

		case 1:
			r = q, g = value, b = p;
			break;

		case 2:
			r = p, g = value, b = t;
			break;

		case 3:
			r = p, g = q, b = value;
			break;

		case 4:
			r = t, g = p, b = value;
			break; 

		case 5:
			r = value, g = p, b = q;
			break;
		}
	}
};