#include "Camera.h"
#include "Shaders/Shader.h"
#include "Graphics.h"
#include "gtx/matrix_decompose.hpp"


//Gathers Data of current frame state from the transform
void Camera::OnPreRender()
{
	//glm::mat4 lightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 20.f);
	//combinedMatrix = lightProj * *transform->GetGlobalMatrix();
	combinedMatrix = projectionMatrix * *transform->GetGlobalMatrix();

	direction = Vector4(transform->Forward() * 1, 0);
	position = Vector4(transform->Position() * -1, 1);

	//std::cout << direction.x << ", " << direction.y << ", " << direction.z << ", " << std::endl;
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
	//shader->SetShaderPass(new ShaderPassMat4(&combinedMatrix, "u_CameraWorldToScreen"));
	//shader->SetShaderPass(new ShaderPassVec4(&position, "u_CameraPosition"));
	//shader->SetShaderPass(new ShaderPassVec4(&direction, "u_CameraDirection"));
	glUniformMatrix4fv(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_CameraWorldToScreen"), 1, GL_FALSE, &combinedMatrix[0][0]);
	glUniform4f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_CameraPosition"), position.x, position.y, position.z, position.w);
	glUniform4f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_CameraDirection"), direction.x, direction.y, direction.z, direction.w);
}

void Camera::GL_SetCameraUniformForSkybox(std::shared_ptr<Shader> shader)
{
	glm::mat4 ccombinedMatrix = projectionMatrix * transform->GetOrientationMatrix();
	//shader->SetShaderPass(new ShaderPassMat4(&ccombinedMatrix, "u_CameraWorldToScreen"));
	shader->SetValueMat4("u_CameraWorldToScreen", ccombinedMatrix);
}
