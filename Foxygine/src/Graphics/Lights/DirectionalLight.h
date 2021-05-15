#pragma once
#include "Light.h"
#include "../../GameObject/Component.h"
#include "glm.hpp"



class DirectionalLight : public Light, public Component
{
private:
    glm::mat4 lightSpaceMatrix;

    void ComposeLightSpaceMatrix();

public:
    int type = (int)LightType::Directional;

    DirectionalLight(Color _color, float _intensity);
    DirectionalLight(Color _color);
    DirectionalLight();
    void GL_SetLightingPasses(int index) override;
    void GL_SetShadowPasses() override;
    bool GetShadowCapabilities() override;
    void OnAttach() override;
    void OnDetach() override;
    void OnTransformChanged() override;
};

