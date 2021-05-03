#pragma once
#include "../GameObject/Transform.h"
#include "../GameObject/Component.h"
#include "GL.h"
#include "../Math/Vector4.h"


class Shader;

class Camera : public Component
{
public:
	glm::mat4 projectionMatrix;
	glm::mat4 combinedMatrix;
	Vector4 position;
	Vector4 direction;

	void OnPreRender() override;
	void SetupCamera(float fov, float screenRatio, float nearClip, float farClip);
	void GL_SetCameraUniform(Shader& shader);
};
