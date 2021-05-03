#pragma once
#include <math.h>
#include "Vector3.h"
#include "Constants.h"
#include "Vector4.h"
#include <../glm/gtc/quaternion.hpp>
#include <glm.hpp>
#include <../glm/common.hpp>


class Quaternion
{
public:
	Quaternion(float _w, float _x, float _y, float _z) {
		w = _w;
		x = _x;
		y = _y;
		z = _z;
	}

	float x;
	float y;
	float z;
	float w;

	Quaternion() 
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Quaternion(float xRot, float yRot, float zRot) {
		SetEuler(xRot, yRot, zRot);
	}

	void SetEuler(float xRot, float yRot, float zRot) {
		float cy = cosf(xRot * 0.5f);
		float sy = sinf(xRot * 0.5f);
		float cp = cosf(yRot * 0.5f);
		float sp = sinf(yRot * 0.5f);
		float cr = cosf(zRot * 0.5f);
		float sr = sinf(zRot * 0.5f);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	}

	Vector3 GetEuler() {
		Vector3 angles;

		// roll (x-axis rotation)
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		angles.z = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		float sinp = 2 * (w * y - z * x);
		if (abs(sinp) >= 1)
			angles.y = copysignf(1.57079632679f, sinp); // use 90 degrees if out of range
		else
			angles.y = asin(sinp);

		// yaw (z-axis rotation)
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		angles.x = atan2(siny_cosp, cosy_cosp);

		return angles;
	}

	Vector3& Rotate(Vector3& vec) 
	{
		Quaternion homVec(0, vec.x, vec.y, vec.z);
		Quaternion result = *this * homVec * this->Inverse();
		return vec = Vector3(result.x, result.y, result.z);
	}

	static Quaternion Conjugate(const Quaternion& q) { Quaternion(q.w, -q.x, -q.y, -q.z); }
	Quaternion& Conjugate() { return *this = Quaternion(w, -x, -y, -z); }

	static float Norm(const Quaternion& q) { return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z; }
	float Norm() { return w * w + x * x + y * y + z * z; }

	static Quaternion Inverse(const Quaternion q) { return Quaternion(Conjugate(q) * Norm(q)); }
	Quaternion& Inverse() {	return *this = this->Conjugate() * this->Norm(); }

	Quaternion operator*(const Quaternion right) const {
		Vector3 leftXYZ(this->x, this->y, this->z);
		Vector3 rightXYZ(right.x, right.y, right.z);
		Vector3 resultXYZ = leftXYZ.Cross(rightXYZ) + rightXYZ * this->w + leftXYZ * right.w;

		return Quaternion(this->w * right.w - leftXYZ.Dot(rightXYZ), resultXYZ.x, resultXYZ.y, resultXYZ.z);
	}

	Quaternion operator*(const float right) const { Quaternion(w * right, x * right, y * right, z * right); }
	operator Vector4() const { return Vector4(x, y, z, w); }
};

