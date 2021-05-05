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


void Camera::SetupCamera(float fov, float screenRatio, float nearClip, float farClip)
{
	projectionMatrix = glm::perspective(fov, screenRatio, nearClip, farClip);
	Graphics::camera = std::shared_ptr<Camera>(this);
}


void Camera::GL_SetCameraUniform(std::shared_ptr<Shader> shader)
{
	shader->SetShaderPass(new ShaderPassMat4(&combinedMatrix, "u_CameraWorldToScreen"));
	shader->SetShaderPass(new ShaderPassVec4(&position, "u_CameraPosition"));
	shader->SetShaderPass(new ShaderPassVec4(&direction, "u_CameraDirection"));
}
