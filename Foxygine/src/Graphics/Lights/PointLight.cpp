#include "PointLight.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"
#include "../Shaders/ShaderPass.h"
#include "../../GameObject/Transform.h"


PointLight::PointLight(Color _color, float _intensity)
{
	color = _color;
	intensity = _intensity;
}


PointLight::PointLight(Color _color)
{
	color = _color;
	intensity = 1;
}


PointLight::PointLight()
{
	color = Color(1, 1, 1, 1.f);
}


void PointLight::GL_SetLightingPasses(int _index)
{
	auto index = std::to_string(_index);
	auto shader = Graphics::GL_GetCurrentlyBoundShader();
	shader->SetShaderPass(new ShaderPassVec1I(&type, "u_LightType[" + index + "]"));
	shader->SetShaderPass(new ShaderPassColor(&color, "u_LightColor[" + index + "]"));
	shader->SetShaderPass(new ShaderPassVec1(&intensity, "u_LightIntensity[" + index + "]"));

	auto position = transform->Position();
	shader->SetShaderPass(new ShaderPassVec3(&position, "u_LightPosition[" + index + "]"));
}


void PointLight::OnAttach()
{
	Graphics::RegisterLight(this);
}


void PointLight::OnDetach()
{
	Graphics::UnregisterLight(this);
}
