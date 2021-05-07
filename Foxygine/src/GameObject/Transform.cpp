#include "Transform.h"
#include "gtx/matrix_decompose.hpp"
#include "gtx/euler_angles.hpp"
#include "gtc/quaternion.hpp"
#include "../glm/common.hpp"



Transform::Transform()
{
	position = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
	globalTransformation = mat4(1);
}

void Transform::SetRotation(Vector3 eulerAngles)
{
}

void Transform::SetRotation(float, float, float)
{
}


void Transform::Rotate(Vector3 axis, float angle)
{
	globalTransformation = rotate(globalTransformation, glm::radians(angle), (vec3)axis);
	extractEulerAngleXYZ(globalTransformation, rotation.x, rotation.y, rotation.z);
}


void Transform::RotateAround(Vector3 pos, Vector3 axis, float angle)
{
	vec3 translation = pos - position;
	globalTransformation = translate(globalTransformation, translation);
	globalTransformation = rotate(globalTransformation, glm::radians(angle), (vec3)axis);
	globalTransformation = translate(globalTransformation, -translation);
	extractEulerAngleXYZ(globalTransformation, rotation.x, rotation.y, rotation.z);
}


void Transform::SetPosition(Vector3 pos)
{
	glm::vec3 translation = pos - position;
	globalTransformation = translate(globalTransformation, translation);
	position = pos;
}


void Transform::SetPosition(float _x, float _y, float _z)
{
	Vector3 pos(_x, _y, _z);
	glm::vec3 translation = pos - position;
	globalTransformation = translate(globalTransformation, translation);
	position = pos;
}


void Transform::Translate(Vector3 translation)
{
	glm::vec3 trans = translation;
	globalTransformation = translate(globalTransformation, trans);
	position = position + translation;
}


void Transform::Translate(float _x, float _y, float _z)
{
	glm::vec3 trans(_x, _y, _z);
	globalTransformation = translate(globalTransformation, trans);
	position = position + trans;
}


glm::mat4 Transform::GetOrientationMatrix()
{
	return glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
}


glm::mat4* Transform::GetGlobalMatrix()
{
	return &globalTransformation;
}


Vector3 Transform::Position()
{
	return position;
}

Vector3 Transform::Rotation()
{
	return rotation;
}

Vector3 Transform::Scale()
{
	return scale;
}
