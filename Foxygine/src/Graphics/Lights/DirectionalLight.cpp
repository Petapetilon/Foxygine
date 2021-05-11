#include "DirectionalLight.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"
#include "../Shaders/ShaderPass.h"
#include "../../GameObject/Transform.h"


DirectionalLight::DirectionalLight(Color _color)
{
    color = _color;
}


DirectionalLight::DirectionalLight()
{
    color = Color(1.f, 1, 1, 1);
}


Light::LightType DirectionalLight::GL_SetLightingPasses(int index)
{
    auto forward = gameObject->transform->Forward() * -1;
    Graphics::GL_GetCurrentlyBoundShader()->SetShaderPass(new ShaderPassVec3(&forward, "u_DirLightDirection[" + std::to_string(index) + "]"));
    Graphics::GL_GetCurrentlyBoundShader()->SetShaderPass(new ShaderPassColor(&color, "u_DirLightColor[" + std::to_string(index) + "]"));

    return LightType::Directional;
}


void DirectionalLight::OnAttach()
{
    Graphics::RegisterLight(this);
}


void DirectionalLight::OnDetach()
{
    Graphics::UnregisterLight(this);
}


