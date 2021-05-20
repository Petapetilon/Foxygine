#include "DirectionalLight.h"
#include "../Graphics.h"
#include "../Shaders/Shader.h"
#include "../../GameObject/Transform.h"
#include "ShadowMap.h"
#include "../Shaders/ShaderLibrary.h"
#include "../Shaders/Shader.h"
#include "../Camera.h"
#include "../../Peripherals/Window.h"
#include <math.h>
#include <limits>


void DirectionalLight::ComposeLightSpaceMatrices()
{
    glm::mat4 lightProj = glm::ortho(-50.f, 50.f, -50.f, 50.f, 1.f, 50.f);
    lightSpaceMatrix = lightProj * *transform->GetGlobalMatrix();



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
        
    
    
        glm::mat4 lightProj = glm::ortho(minX, maxX, minY, maxY, minZ, 100.f);
        lightSpaceMatrices[i] = lightProj * *transform->GetGlobalMatrix();
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
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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


void DirectionalLight::GL_SetLightingPasses(int _index)
{
    //ComposeLightSpaceMatrices();
    auto index = std::to_string(_index);
    auto shader = Graphics::GL_GetCurrentlyBoundShader();

    shader->SetValueVec1I("u_LightType[" + index + "]", type);
    shader->SetValueColor("u_LightColor[" + index + "]", color);
    shader->SetValueVec1("u_LightIntensity[" + index + "]", intensity);

    auto forward = gameObject->transform->Forward();
    shader->SetValueVec3("u_LightDirection[" + index + "]", forward);
    shader->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrix);


    shader->SetValueMat4("u_LSM[0]", lightSpaceMatrices[0]);
    shader->SetValueMat4("u_LSM[1]", lightSpaceMatrices[1]);
    shader->SetValueMat4("u_LSM[2]", lightSpaceMatrices[2]);
    shader->SetValueMat4("u_LSM[3]", lightSpaceMatrices[3]);


    GL_Call(glActiveTexture(GL_TEXTURE9));
    glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[0]);
    //GL_Call(glUniform1i(glGetUniformLocation(Graphics::GL_GetCurrentlyBoundShader()->GL_GetShaderProgram(), "u_SM1"), 9));
    GL_Call(glActiveTexture(GL_TEXTURE10));
    glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[1]);
    //GL_Call(glUniform1i(glGetUniformLocation(Graphics::GL_GetCurrentlyBoundShader()->GL_GetShaderProgram(), "u_SM2"), 9));
    GL_Call(glActiveTexture(GL_TEXTURE11));
    glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[2]);
    //GL_Call(glUniform1i(glGetUniformLocation(Graphics::GL_GetCurrentlyBoundShader()->GL_GetShaderProgram(), "u_SM3"), 9));
    GL_Call(glActiveTexture(GL_TEXTURE12));
    glBindTexture(GL_TEXTURE_2D, GL_ShadowMapObject[3]);
    //GL_Call(glUniform1i(glGetUniformLocation(Graphics::GL_GetCurrentlyBoundShader()->GL_GetShaderProgram(), "u_SM4"), 9));

}


void DirectionalLight::GL_RenderShadowMap()
{
    ComposeLightSpaceMatrices();
    Graphics::GL_GetCurrentlyBoundShader()->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrix);


    //shadowMapShader->GL_BindProgram();
    //
    //GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, GL_FrambufferObject));
    //glViewport(0, 0, GL_ShadowMapResolution, GL_ShadowMapResolution);
    //glDepthFunc(GL_LESS);
    //glCullFace(GL_FRONT);
    //for (int i = 0; i < CSMSplits; i++) {
    //    GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_ShadowMapObject[i], 0));
    //    glDrawBuffer(GL_NONE);
    //    glReadBuffer(GL_NONE);
    //    glClear(GL_DEPTH_BUFFER_BIT);
    //    shadowMapShader->SetValueMat4("u_LightSpaceMatrix", lightSpaceMatrices[i]);
    //    Graphics::RenderShadowPass(this);
    //}
    //
    //GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    //auto screenRes = Window::GetInstance()->GetWindowResolution();
    //glViewport(0, 0, screenRes.x, screenRes.y);
    //glDepthFunc(GL_LEQUAL);
    //glCullFace(GL_BACK);
}


bool DirectionalLight::GetShadowCapabilities()
{
    return true;
}


void DirectionalLight::OnAttach()
{
    Graphics::RegisterLight(this);
    ComposeLightSpaceMatrices();

    shadowMapShader = std::shared_ptr<Shader>(ShaderLibrary::GetShader("Internal_CSM_Shadow_Shader"));
    shadowMapShader = std::shared_ptr<Shader>(Shader::CreateEmptyShader("Internal_CSM_ShadowMap_Shader"));
    shadowMapShader->LoadShaderResource("res\\VertexShader\\ShadowDepthMapVert.vert", Shader::ShaderType::VertexShader);
    shadowMapShader->LoadShaderResource("res\\FragmentShader\\ShadowDepthMapFrag.frag", Shader::ShaderType::FragmentShader);

    CSMSplits = -1;
    GL_Call(glGenFramebuffers(1, &GL_FrambufferObject));
    SetShadowMapResolution(Graphics::CSM_ShadowResolution);
}


void DirectionalLight::OnDetach()
{
    Graphics::UnregisterLight(this);
}


void DirectionalLight::OnTransformChanged()
{
    ComposeLightSpaceMatrices();
}
