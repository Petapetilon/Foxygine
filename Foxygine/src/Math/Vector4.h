#pragma once


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
};