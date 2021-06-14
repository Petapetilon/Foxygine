#pragma once
#include "Light.h"
#include "../../GameObject/Component.h"


class PointLight : public Light, public Component
{
private:
    bool drawShadow;

public:
    const int type = (int)Lighting::LightType::Point;

    PointLight(Color _color, float _intensity, bool _drawShadow = false);
    PointLight(Color _color, bool _drawShadow = false);
    PointLight();
    void GL_SetLightingPasses(int index) override;
    bool GL_FillLightPass(Lighting::LightPass* pass) override;
    void GL_RenderShadowMap() override;
    bool GetShadowCapabilities() override;

    Component* Copy(std::size_t& compHash) override;
    void OnAttach() override;
    void OnDetach() override;
};

