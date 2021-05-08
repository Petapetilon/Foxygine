#include "Camera.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderPass.h"
#include "Graphics.h"
#include "gtx/matrix_decompose.hpp"


//Gathers Data of current frame state from the transform
void Camera::OnPreRender()
{
	combinedMatrix = projectionMatrix * *transform->GetGlobalMatrix();

	mat4 inverted = glm::inverse(*transform->GetGlobalMatrix());
	direction = Vector4(normalize(glm::vec3(inverted[2])), 1);
	position = Vector4(transform->Position(), 1);
}


void Camera::SetupCamera(float _fov, float _screenRatio, float _nearClip, float _farClip)
{
	FOV = _fov;
	nearClippingDistance = _nearClip;
	farClippingDistance = _farClip;
	projectionMatrix = glm::perspective(_fov, _screenRatio, _nearClip, _farClip);
	Graphics::camera = std::shared_ptr<Camera>(this);
}

void Camera::ResetCamera(float _screenRatio)
{
	projectionMatrix = glm::perspective(FOV, _screenRatio, nearClippingDistance, farClippingDistance);
}


void Camera::GL_SetCameraUniform(std::shared_ptr<Shader> shader)
{
	combinedMatrix = projectionMatrix * *transform->GetGlobalMatrix();
	shader->SetShaderPass(new ShaderPassMat4(&combinedMatrix, "u_CameraWorldToScreen"));
	shader->SetShaderPass(new ShaderPassVec4(&position, "u_CameraPosition"));
	shader->SetShaderPass(new ShaderPassVec4(&direction, "u_CameraDirection"));
}

void Camera::GL_SetCameraUniformForSkybox(std::shared_ptr<Shader> shader)
{
	glm::mat4 ccombinedMatrix = projectionMatrix * transform->GetOrientationMatrix();
	shader->SetShaderPass(new ShaderPassMat4(&ccombinedMatrix, "u_CameraWorldToScreen"));
}
