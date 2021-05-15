#include "Graphics.h"
#include "MeshRenderer.h"
#include "SkyBoxRenderer.h"
#include "Lights/Light.h"
#include "Shaders/ShaderPass.h"
#include "Lights/ShadowMap.h"
#include "Camera.h"
#include "Material.h"



SkyboxRenderer* Graphics::skyBoxRenderer;
Shader* Graphics::currentlyBoundShader;
std::shared_ptr<Camera> Graphics::camera;
long Graphics::renderedFrames;

std::list<MeshRenderer*> Graphics::meshRenderers;
std::list<Light*> Graphics::lights;




void Graphics::RenderShadowPrePassForwarded()
{
	for (auto light : lights) {
		//std::cout << "render this crap for this light" << std::endl;
		ShadowMap::RenderShadowMap(light);
	}
}


void Graphics::RenderUnlitPassForwarded()
{
}


void Graphics::RenderLitPassForwarded()
{
	if (camera == nullptr) {
		std::cout << "No active Camera found! Can not Render Frames!" << std::endl;
		return;
	}

	//std::cout << "drawing god damn scene shaded" << std::endl;

	for (const auto& renderer : meshRenderers) {
		renderer->Draw(camera);
	}

	skyBoxRenderer->Draw(camera);
}


void Graphics::RenderUIPassForwarded()
{
}


void Graphics::Init()
{
	currentlyBoundShader = nullptr;
	skyBoxRenderer = nullptr;
	camera = nullptr;
	renderedFrames = 0;

	ShadowMap::InitShadowMap();
}


void Graphics::RenderFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderShadowPrePassForwarded();
	//RenderUnlitPassForwarded();
	RenderLitPassForwarded();

	//RenderUIPassForwarded();

	//if(skyBoxRenderer != nullptr)
		//skyBoxRenderer->Draw(camera);

	renderedFrames++;
}


void Graphics::RegisterMeshRenderer(MeshRenderer* meshRenderer) {
	meshRenderers.push_back(meshRenderer);
}


void Graphics::UnregeisterMeshRenderer(MeshRenderer* meshRenderer) {
	meshRenderers.remove(meshRenderer);
}


void Graphics::RegisterLight(Light* light)
{
	lights.push_back(light);
}


void Graphics::UnregisterLight(Light* light)
{
	lights.remove(light);
}


void Graphics::SetSkybox(std::vector<std::string> filePaths)
{
	if (skyBoxRenderer == nullptr) {
		skyBoxRenderer = new SkyboxRenderer;
	}

	skyBoxRenderer->SetSkybox(filePaths);
}

void Graphics::OnWindowResize(int width, int height)
{
	camera->ResetCamera(width / height);
	glViewport(0, 0, width, height);
	RenderFrame();
}


void Graphics::FinishLoadingResources()
{
	for (auto mR : meshRenderers) {
		mR->GetMaterial()->FinishLoadingResources();
	}

	skyBoxRenderer->GetMaterial()->FinishLoadingResources();
}


Shader* Graphics::GL_GetCurrentlyBoundShader()
{
	return currentlyBoundShader;
}


void Graphics::GL_SetCurrentlyBoundShader(Shader* shader)
{
	if (shader == nullptr) return;

	int lightCount = lights.size();
	int k = 0;
	currentlyBoundShader = shader;
	if (currentlyBoundShader->GetShaderLitType()) {
		for (auto light : lights) {
			light->GL_SetLightingPasses(k++);
			//light->GL_SetShadowPasses();
		}

		//std::cout << "binding fucking texture again for this fucking shader: " << shader->name << std::endl;
		currentlyBoundShader->SetShaderPass(new ShaderPassVec1I(&lightCount, "u_NumberLights"));
		ShadowMap::GL_BindShadowMap();
	}

	//if (renderedFrames != 0) {
	//	camera->GL_SetCameraUniform(std::shared_ptr<Shader>(currentlyBoundShader));
	//}
}


void Graphics::RenderShadowPass(Light* light)
{
	//std::cout << "filing fucking buffer" << std::endl;
	for (auto renderer : meshRenderers) {
		renderer->DrawShadowMap(light);
	}
}
