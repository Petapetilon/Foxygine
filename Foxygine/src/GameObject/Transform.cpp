#include "Transform.h"
#include "gtx/matrix_decompose.hpp"
#include "gtx/rotate_vector.hpp"
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


void Transform::SetRotation(float x , float y, float z, bool localy)
{
	globalTransformation = mat4(1);
	auto rotationMatrix = rotate(glm::radians(x), glm::vec3(1, 0, 0));
	rotationMatrix *= rotate(glm::radians(y), glm::vec3(0, 1, 0));
	rotationMatrix *= rotate(glm::radians(z), glm::vec3(0, 0, 1));
	if (localy) {
		globalTransformation = globalTransformation * rotationMatrix;
		globalTransformation = translate(globalTransformation, (vec3)position);
	}
	else {
		globalTransformation = rotationMatrix * globalTransformation;
		globalTransformation = translate(globalTransformation, (vec3)position);
	}
}


void Transform::Rotate(Vector3 axis, float angle)
{
	//globalTransformation = translate(globalTransformation, -(vec3)position);
	globalTransformation = rotate(globalTransformation, glm::radians(angle), (vec3)axis);
	//globalTransformation = translate(globalTransformation, (vec3)position);
	extractEulerAngleXYZ(globalTransformation, rotation.x, rotation.y, rotation.z);
}

void Transform::Rotate(float x, float y, float z, bool localy)
{
	globalTransformation = mat4(1);
	auto rotationMatrix = rotate(x, glm::vec3(1, 0, 0));
	rotationMatrix *= rotate(y, glm::vec3(0, 1, 0));
	rotationMatrix *= rotate(z, glm::vec3(0, 0, 1));
	if (localy) {
		globalTransformation = globalTransformation * rotationMatrix;
		globalTransformation = translate(globalTransformation, (vec3)position);
	}
	else {
		globalTransformation = rotationMatrix * globalTransformation;
		globalTransformation = translate(globalTransformation, (vec3)position);
	}
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


Vector3 Transform::Forward()
{
	mat4 inverted = glm::inverse(globalTransformation);
	return Vector3(normalize(glm::vec3(inverted[2])));
}

Vector3 Transform::Right()
{
	mat4 inverted = glm::inverse(globalTransformation);
	return Vector3(-normalize(glm::vec3(inverted[0])));
}

Vector3 Transform::Up()
{
	mat4 inverted = glm::inverse(globalTransformation);
	return Vector3(-normalize(glm::vec3(inverted[1])));
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
