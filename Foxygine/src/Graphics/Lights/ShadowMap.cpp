#include "ShadowMap.h"
#include "../Textures/Texture2D.h"
#include "../Graphics.h"
#include "../../Peripherals/Window.h"
#include "../Shaders/Shader.h"
#include "Light.h"
#include "../../GameObject/GameObjectHandler.h"
#include "../../GameObject/GameObject.h"
#include "../MeshRenderer.h"



std::vector<Texture2D*> ShadowMap::shadowMaps;
//unsigned int ShadowMap::sm.GL_ShadowMapFrameBufferObject;
//unsigned int ShadowMap::sm.GL_ShadowDepthMap;
unsigned int ShadowMap::shadowDepthMapResolution;
long ShadowMap::currentlyRenderedFrame;
short ShadowMap::alreadyRenderedShadowMaps;
std::shared_ptr<Shader> ShadowMap::shadowMapShader;
ShadowMap ShadowMap::sm;


void ShadowMap::InitShadowMap()
{
	sm = ShadowMap();

	shadowMapShader = std::shared_ptr<Shader>(Shader::CreateEmptyShader("Internal_ShadowMap_Shader"));
	shadowMapShader->LoadShaderResource("res\\VertexShader\\ShadowDepthMapVert.vert", Shader::ShaderType::VertexShader);
	shadowMapShader->LoadShaderResource("res\\FragmentShader\\ShadowDepthMapFrag.frag", Shader::ShaderType::FragmentShader);

	SetShadowMapResolution(4096);
}

void ShadowMap::FreeShadowMap()
{
}

void ShadowMap::SetShadowMapResolution(unsigned int resolution)
{
	shadowDepthMapResolution = resolution;

	GL_Call(glGenFramebuffers(1, &sm.GL_ShadowMapFrameBufferObject));
	GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, sm.GL_ShadowMapFrameBufferObject));
	
	glGenTextures(1, &sm.GL_ShadowDepthMap);
	glBindTexture(GL_TEXTURE_2D, sm.GL_ShadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowDepthMapResolution, shadowDepthMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sm.GL_ShadowDepthMap, 0));

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	GLenum status2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status2 != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Incomplete framebuffer object " << status2 << std::endl;
	}

	GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}


void ShadowMap::RenderShadowMap(Light* light)
{
	//if (!light->GetShadowCapabilities()) return;

	//if (currentlyRenderedFrame < Graphics::renderedFrames) {
	//	alreadyRenderedShadowMaps = 0;
	//	currentlyRenderedFrame = Graphics::renderedFrames;
	//}

	//if (alreadyRenderedShadowMaps >= ShadowMap::shadowMapCount) return;


	GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, sm.GL_ShadowMapFrameBufferObject));
	glViewport(0, 0, shadowDepthMapResolution, shadowDepthMapResolution);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);

	
	if ((glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE))
		__debugbreak();


	shadowMapShader->GL_BindProgram();
	light->GL_SetShadowPasses();
	Graphics::RenderShadowPass(light);
	
	
	GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto screenRes = Window::GetInstance()->GetWindowResolution();
	glViewport(0, 0, screenRes.x, screenRes.y);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);

	alreadyRenderedShadowMaps++;
}


std::shared_ptr<Shader> ShadowMap::GetShadowMapShader()
{
	return std::shared_ptr<Shader>(shadowMapShader);
}


void ShadowMap::GL_BindShadowMap()
{
	sm.alreadyRenderedShadowMaps;
	GL_Call(glActiveTexture(GL_TEXTURE8));
	glBindTexture(GL_TEXTURE_2D, sm.GL_ShadowDepthMap);
}
