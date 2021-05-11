#pragma once
#include "Light.h"
#include "../../GameObject/Component.h"



class DirectionalLight : public Light, public Component
{
public:
    DirectionalLight(Color _color);
    DirectionalLight();
    LightType GL_SetLightingPasses(int index) override;
    void OnAttach() override;
    void OnDetach() override;
};

