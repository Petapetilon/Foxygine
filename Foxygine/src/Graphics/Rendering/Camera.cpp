#include "Camera.h"
#include "../Shaders/Shader.h"
#include "../Graphics.h"
#include "gtx/matrix_decompose.hpp"


//Gathers Data of current frame state from the transform
void Camera::OnPreRender()
{
	combinedMatrix = projectionMatrix * *transform->GetGlobalMatrix();

	direction = Vector4(transform->Forward() * 1, 0);
	position = Vector4(transform->Position() * -1, 1);
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
	glUniformMatrix4fv(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_CameraWorldToScreen"), 1, GL_FALSE, &combinedMatrix[0][0]);
	glUniform4f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_CameraPosition"), position.x, position.y, position.z, position.w);
	glUniform4f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_CameraDirection"), direction.x, direction.y, direction.z, direction.w);
}


void Camera::GL_SetCameraUniformForSkybox(std::shared_ptr<Shader> shader)
{
	glm::mat4 ccombinedMatrix = projectionMatrix * transform->GetOrientationMatrix();
	shader->SetValueMat4("u_CameraWorldToScreen", ccombinedMatrix);
}
