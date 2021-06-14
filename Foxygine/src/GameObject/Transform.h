#pragma once
#include "glm.hpp"
#include "mat4x4.hpp"
#include "gtc/matrix_transform.hpp"
#include "trigonometric.hpp"
#include "gtc/matrix_access.hpp"

#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
#include "GameObject.h"


using namespace glm;


class Transform
{
private:
	mat4 globalTransformation;
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

public:
	GameObject* gameObject;

	Transform();

	void CopyTo(Transform* transform);

	void SetRotation(Vector3);
	void SetRotation(float, float, float, bool);
	void Rotate(Vector3, float);
	void Rotate(float x, float y, float z, bool localy);
	void RotateAround(Vector3, Vector3, float);

	void SetPosition(Vector3);
	void SetPosition(float, float, float);
	void Translate(Vector3);
	void Translate(float, float, float);
	void Scale(Vector3);

	Vector3 Forward();
	Vector3 Right();
	Vector3 Up();
	Vector3 Position();
	Vector3 Rotation();
	Vector3 Scale();

	glm::mat4 GetOrientationMatrix();
	glm::mat4* GetGlobalMatrix();
};

