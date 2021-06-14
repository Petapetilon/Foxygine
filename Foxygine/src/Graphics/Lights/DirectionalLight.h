#pragma once
#include "Light.h"
#include "../../GameObject/Component.h"
#include "glm.hpp"
#include "../GL.h"

class Shader;
class Color;


class DirectionalLight : public Light, public Component
{
private:
    struct SplitFurstumInfo {
    public:
        float right;
        float left;
        float bottom;
        float top;
        float far;
        float near;
    };

    glm::mat4 lightSpaceMatrix;
    glm::mat4* lightSpaceMatrices;
    GLuint GL_FrambufferObject;
    GLuint* GL_ShadowMapObject;
    SplitFurstumInfo* frustumInfos;
    unsigned int GL_ShadowMapResolution;
    unsigned int CSMSplits;
    std::shared_ptr<Shader> shadowMapShader;
    bool useCSM;
    bool drawShadow;
    Vector3 position;

    void ComposeLightSpaceMatrices();

public:
    const int type = (int)Lighting::LightType::Directional;

    DirectionalLight(Color _color, float _intensity);
    DirectionalLight(Color _color);
    DirectionalLight();
    void SetShadowMapResolution(unsigned int resolution) override;
    void SetShadowingTechnique(bool _useCSM);


    void GL_SetLightingPasses(int index) override;
    bool GL_FillLightPass(Lighting::LightPass* pass) override;
    void GL_RenderShadowMap() override;
    bool GetShadowCapabilities() override;
    void OnAttach() override;
    void OnDetach() override;
    void OnPreRender() override;
    void OnTransformChanged() override;
};

