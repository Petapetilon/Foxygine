#pragma once
#include "Light.h"
#include "../../GameObject/Component.h"


class PointLight : public Light, public Component
{
private:
    int type = (int)LightType::Point;

public:
    PointLight(Color _color, float _intensity);
    PointLight(Color _color);
    PointLight();
    void GL_SetLightingPasses(int index) override;
    void OnAttach() override;
    void OnDetach() override;
};

