#pragma once
#include <memory>
#include "../../GameObject/Transform.h"
#include "../../GameObject/Component.h"
#include "../GL.h"
#include "../../Math/Vector4.h"


class Shader;

class Camera : public Component
{
public:
	glm::mat4 projectionMatrix;
	glm::mat4 combinedMatrix;
	Vector4 position;
	Vector4 direction;
	float FOV;
	float nearClippingDistance;
	float farClippingDistance;

	void OnPreRender() override;
	void SetupCamera(float _fov, float _screenRatio, float _nearClip, float _farClip);
	void ResetCamera(float _screenRatio);
	void GL_SetCameraUniform(std::shared_ptr<Shader> shader);
	void GL_SetCameraUniformForSkybox(std::shared_ptr<Shader> shader);
};
