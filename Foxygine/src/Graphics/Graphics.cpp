#include "Graphics.h"
#include "MeshRenderer.h"
#include "SkyBoxRenderer.h"
#include "Lights/Light.h"
#include "Shaders/ShaderPass.h"
#include <thread>


SkyboxRenderer* Graphics::skyBoxRenderer;
Shader* Graphics::currentlyBoundShader;
std::shared_ptr<Camera> Graphics::camera;

std::list<MeshRenderer*> Graphics::meshRenderers;
std::list<Light*> Graphics::lights;

int Graphics::numberOfDirLights;
int Graphics::numberOfPointLights;

long Graphics::renderedFrames;


void Graphics::Init()
{
	currentlyBoundShader = nullptr;
	skyBoxRenderer = nullptr;
	camera = nullptr;
	renderedFrames = 0;

}

void Graphics::DrawRenderer() {
	if (camera == nullptr) {
		std::cout << "No active Camera found! Can not Render Frames!" << std::endl;
		return;
	}

	for (const auto& renderer : meshRenderers) {
		renderer->Draw(camera);
	}

	skyBoxRenderer->Draw(camera);
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
	DrawRenderer();
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

	numberOfDirLights = 0;
	numberOfPointLights = 0;
	int k = 0;

	currentlyBoundShader = shader;
	if (currentlyBoundShader->GetShaderLitType()) {
		for (auto light : lights) {
			auto type = light->GL_SetLightingPasses(k++);
			if (type == Light::LightType::Directional) {
				++numberOfDirLights;
			}
			else {
				++numberOfPointLights;
			}
		}

		currentlyBoundShader->SetShaderPass(new ShaderPassVec1I(&numberOfDirLights, "u_NumberDirLights"));
		currentlyBoundShader->SetShaderPass(new ShaderPassVec1I(&numberOfPointLights, "u_NumberPointLights"));
	}
}
