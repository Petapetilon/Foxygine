#include "DirectionalLight.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"
#include "../../GameObject/Transform.h"
#include "ShadowMap.h"


void DirectionalLight::ComposeLightSpaceMatrix()
{
    glm::mat4 lightProj = glm::ortho(-50.f, 50.f, -50.f, 50.f, 1.f, 50.f);
    //lightProj = glm::perspective(glm::radians(60.f), 1.f, 1.f, 20.f);
    //glm::mat4 view = glm::lookAt((vec3)(transform->Position()), glm::vec3(0.f, 0.f, 0.f), normalize(glm::vec3(1, 1, 0)));
    //glm::mat4 view = glm::lookAt<float>(glm::vec3(0, 0, 0), (vec3)(transform->Position()), glm::vec3(0, 1, 0));
    //view = glm::transpose(glm::inverse(view));
    //lightSpaceMatrix = lightProj * view;
    lightSpaceMatrix = lightProj * *transform->GetGlobalMatrix();


    //glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
    //    glm::vec3(0.0f, 0.0f, 0.0f),
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    //
    //lightSpaceMatrix = lightProj * lightView;
}


DirectionalLight::DirectionalLight(Color _color, float _intensity)
{
    color = _color;
    intensity = _intensity;
}


DirectionalLight::DirectionalLight(Color _color)
{
    color = _color;
    intensity = 1;
}


DirectionalLight::DirectionalLight()
{
    color = Color(1.f, 1, 1, 1);
    intensity = 1;
}


void DirectionalLight::GL_SetLightingPasses(int _index)
{
    ComposeLightSpaceMatrix();
    auto index = std::to_string(_index);
    auto shader = Graphics::GL_GetCurrentlyBoundShader();

    shader->SetValueVec1I("u_LightType[" + index + "]", type);
    shader->SetValueColor("u_LightColor[" + index + "]", color);
    shader->SetValueVec1("u_LightIntensity[" + index + "]", intensity);

    auto forward = gameObject->transform->Forward();
    shader->SetValueVec3("u_LightDirection[" + index + "]", forward);
    shader->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrix);
}


void DirectionalLight::GL_SetShadowPasses()
{
    ComposeLightSpaceMatrix();
    ShadowMap::GetShadowMapShader()->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrix);
}


bool DirectionalLight::GetShadowCapabilities()
{
    return true;
}


void DirectionalLight::OnAttach()
{
    Graphics::RegisterLight(this);
    ComposeLightSpaceMatrix();
}


void DirectionalLight::OnDetach()
{
    Graphics::UnregisterLight(this);
}


void DirectionalLight::OnTransformChanged()
{
    ComposeLightSpaceMatrix();
}
