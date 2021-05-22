#include "DirectionalLight.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"
#include "../../GameObject/Transform.h"
#include "../Shaders/ShaderLibrary.h"
#include "../Shaders/Shader.h"
#include "../Rendering/Camera.h"
#include "../../Peripherals/Window.h"
#include <math.h>
#include <limits>


void DirectionalLight::ComposeLightSpaceMatrices()
{
    if (!useCSM) {
        glm::mat4 lightProj = glm::ortho(-50.f, 50.f, -50.f, 50.f, 1.f, 200.f);
        lightSpaceMatrix = lightProj * *transform->GetGlobalMatrix();

        Vector3 cameraPositionPlane = Graphics::camera->transform->Position();
        cameraPositionPlane.y = 0;
        lightSpaceMatrix = translate(lightSpaceMatrix, (vec3)(transform->Forward() * -100 + cameraPositionPlane - transform->Position()));
    }
    else {
        float* splitLocs = new float[5];
        splitLocs[0] = Graphics::camera->nearClippingDistance;
        splitLocs[1] = 25;
        splitLocs[2] = 100;
        splitLocs[3] = 300;
        splitLocs[4] = Graphics::camera->farClippingDistance;
        
        
        // Get the inverse of the view transform
        glm::mat4 inverseCameraTransform = inverse(*Graphics::camera->transform->GetGlobalMatrix());
        
        // Get the light space tranform
        glm::mat4 lightViewDirection = lookAt(vec3(0, 0, 0), (vec3)transform->Forward(), vec3(0, 1, 0));
        //glm::mat4 lightViewDirection = transform->GetOrientationMatrix();
        
        float ar = (float)Window::GetInstance()->GetWindowResolution().y / (float)Window::GetInstance()->GetWindowResolution().x;
        float tanHalfHFOV = tanf(glm::radians(Graphics::camera->FOV / 2.0f));
        float tanHalfVFOV = tanf(glm::radians((Graphics::camera->FOV * ar) / 2.0f));
        
        for (uint i = 0; i < CSMSplits; i++) {
            float xn = splitLocs[i] * tanHalfHFOV;
            float xf = splitLocs[i + 1] * tanHalfHFOV;
            float yn = splitLocs[i] * tanHalfVFOV;
            float yf = splitLocs[i + 1] * tanHalfVFOV;
        
            vec4 frustumCorners[8] = {
                // near face
                vec4(xn, yn, splitLocs[i], 1.0),
                vec4(-xn, yn, splitLocs[i], 1.0),
                vec4(xn, -yn, splitLocs[i], 1.0),
                vec4(-xn, -yn, splitLocs[i], 1.0),
        
                // far face
                vec4(xf, yf, splitLocs[i + 1], 1.0),
                vec4(-xf, yf, splitLocs[i + 1], 1.0),
                vec4(xf, -yf, splitLocs[i + 1], 1.0),
                vec4(-xf, -yf, splitLocs[i + 1], 1.0)
            };
        
            vec4 frustumCornersL[8];
        
            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::min();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::min();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = std::numeric_limits<float>::min();
        
            for (uint j = 0; j < 8; j++) {
        
                // Transform the frustum coordinate from view to world space
                vec4 vW = inverseCameraTransform * frustumCorners[j];
        
                // Transform the frustum coordinate from world to light space
                frustumCornersL[j] = lightViewDirection * vW;
        
               
                minX = min(minX, frustumCornersL[j].x);
                maxX = max(maxX, frustumCornersL[j].x);
                minY = min(minY, frustumCornersL[j].y);
                maxY = max(maxY, frustumCornersL[j].y);
                minZ = min(minZ, frustumCornersL[j].z);
                maxZ = max(maxZ, frustumCornersL[j].z);
            }
        
            frustumInfos[i].left = minX;
            frustumInfos[i].right = maxX;
            frustumInfos[i].bottom = minY;
            frustumInfos[i].top = maxY;
            frustumInfos[i].near = minZ;
            frustumInfos[i].far = maxZ;
            
        
        
            glm::mat4 lightProj = glm::ortho(minX, maxX, minY, maxY, 1.f, 100.f);
            lightSpaceMatrices[i] = lightProj * *transform->GetGlobalMatrix();
        }
    }
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


void DirectionalLight::SetShadowMapResolution(unsigned int resolution)
{
    if (useCSM) {
        if (CSMSplits == -1)
            CSMSplits = 4;
        else
            for (int i = 0; i < CSMSplits; i++)
                glDeleteTextures(1, GL_ShadowMapObject + i);

        frustumInfos = new SplitFurstumInfo[CSMSplits];
        lightSpaceMatrices = new glm::mat4[CSMSplits];
        GL_ShadowMapObject = new GLuint[CSMSplits];
        GL_ShadowMapResolution = resolution;

        for (int i = 0; i < CSMSplits; i++) {
            glGenTextures(1, GL_ShadowMapObject + i);
            glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[i]);

            glTexImage2D(GL_TEXTURE_2D, i, GL_DEPTH_COMPONENT, GL_ShadowMapResolution, GL_ShadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, GL_FrambufferObject);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_ShadowMapObject[0], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status2 != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Incomplete framebuffer object " << status2 << std::endl;
        }

        GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    else {
        if(GL_ShadowMapObject != nullptr)
            glDeleteTextures(1, GL_ShadowMapObject);

        GL_ShadowMapObject = new GLuint[1];
        GL_ShadowMapResolution = resolution;

        glBindFramebuffer(GL_FRAMEBUFFER, GL_FrambufferObject);

        glGenTextures(1, &GL_ShadowMapObject[0]);
        glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[0]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_ShadowMapResolution, GL_ShadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_ShadowMapObject[0], 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status2 != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Incomplete framebuffer object " << status2 << std::endl;
        }

        GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}


void DirectionalLight::SetShadowingTechnique(bool _useCSM)
{
    useCSM = _useCSM;
    SetShadowMapResolution(GL_ShadowMapResolution);
}


void DirectionalLight::GL_SetLightingPasses(int _index)
{
    auto index = std::to_string(_index);
    auto shader = Graphics::GL_GetCurrentlyBoundShader();

    shader->SetValueVec1I("u_LightType[" + index + "]", type);
    shader->SetValueColor("u_LightColor[" + index + "]", color);
    shader->SetValueVec1("u_LightIntensity[" + index + "]", intensity);

    auto forward = gameObject->transform->Forward();
    shader->SetValueVec3("u_LightDirection[" + index + "]", forward);

    if (!useCSM) {
        shader->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrix);
        GL_Call(glActiveTexture(GL_TEXTURE8));
        glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[0]);
    }
}


void DirectionalLight::GL_RenderShadowMap()
{
    shadowMapShader->GL_BindProgram();
    
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, GL_FrambufferObject));
    glViewport(0, 0, GL_ShadowMapResolution, GL_ShadowMapResolution);
    glDepthFunc(GL_LESS);
    glCullFace(GL_FRONT);
    if (useCSM) {
        for (int i = 0; i < CSMSplits; i++) {
            GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_ShadowMapObject[i], 0));
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glClear(GL_DEPTH_BUFFER_BIT);
            shadowMapShader->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrices[i]);
            Graphics::RenderShadowPass(this);
        }
    }
    else {
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowMapShader->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrix);
        Graphics::RenderShadowPass(this);
    }
    
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    auto screenRes = Window::GetInstance()->GetWindowResolution();
    glViewport(0, 0, screenRes.x, screenRes.y);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);    
}


bool DirectionalLight::GetShadowCapabilities()
{
    return true;
}


void DirectionalLight::OnAttach()
{
    Graphics::RegisterLight(this);
    ComposeLightSpaceMatrices();

    shadowMapShader = std::shared_ptr<Shader>(ShaderLibrary::GetShader("Internal_Directional_Shadow_Shader"));
    shadowMapShader = std::shared_ptr<Shader>(Shader::CreateEmptyShader("Internal_Directional_Shadow_Shader"));
    shadowMapShader->LoadShaderResource("res\\VertexShader\\ShadowDepthMapVert.vert", Shader::ShaderType::VertexShader);
    shadowMapShader->LoadShaderResource("res\\FragmentShader\\ShadowDepthMapFrag.frag", Shader::ShaderType::FragmentShader);

    CSMSplits = -1;
    GL_ShadowMapObject = nullptr;
    GL_Call(glGenFramebuffers(1, &GL_FrambufferObject));
    GL_ShadowMapResolution = Graphics::CSM_ShadowResolution;
    SetShadowingTechnique(false);
}


void DirectionalLight::OnDetach()
{
    Graphics::UnregisterLight(this);
}

void DirectionalLight::OnPreRender()
{
    ComposeLightSpaceMatrices();
}


void DirectionalLight::OnTransformChanged()
{
    ComposeLightSpaceMatrices();
}
