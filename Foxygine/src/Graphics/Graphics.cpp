#include "Graphics.h"
#include "Mesh/MeshRenderer.h"
#include "Mesh/SkyBoxRenderer.h"
#include "Lights/Light.h"
#include "Rendering/Camera.h"
#include "Rendering/Material.h"
#include "../Peripherals/Window.h"
#include "../Graphics/UI/Canvas.h"
#include "Lights/Lighting.h"



SkyboxRenderer* Graphics::skyBoxRenderer;
Shader* Graphics::currentlyBoundShader;
std::shared_ptr<Camera> Graphics::camera;
long Graphics::renderedFrames;
int Graphics::CSM_ShadowResolution;
int Graphics::PointLight_ShadowResolution;
int Graphics::SpotLight_ShadowResolution;

std::list<MeshRenderer*> Graphics::meshRenderers;
std::list<Canvas*> Graphics::canvases;
std::list<Light*> Graphics::lights;




void Graphics::RenderShadowPrePassForwarded()
{
	for (auto light : lights) {
		light->GL_RenderShadowMap();
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
	for (auto canvas : canvases) {
		canvas->Draw();
	}
}


void Graphics::Init()
{
	currentlyBoundShader = nullptr;
	skyBoxRenderer = nullptr;
	camera = nullptr;
	renderedFrames = 0;
	CSM_ShadowResolution = 4096;
	PointLight_ShadowResolution = 2048;
	SpotLight_ShadowResolution = 2048;

	Window::GetInstance()->SetWindowResizeCallback(Graphics::OnWindowResize);

	Shader::CreateBasicLitShader("Basic Lit");
	Shader::CreateBasicUnlitUIShader("Basic Unlit UI");
}


void Graphics::RenderFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderShadowPrePassForwarded();
	//RenderUnlitPassForwarded();
	RenderLitPassForwarded();

	if(skyBoxRenderer != nullptr)
		skyBoxRenderer->Draw(camera);

	RenderUIPassForwarded();

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


void Graphics::RegisterCanvas(Canvas* canvas)
{
	canvases.push_back(canvas);
}


void Graphics::UnregisterCanvas(Canvas* canvas)
{
	canvases.remove(canvas);
}


void Graphics::SetSkybox(std::vector<std::string> filePaths)
{
	if (skyBoxRenderer == nullptr) {
		skyBoxRenderer = new SkyboxRenderer;
	}

	skyBoxRenderer->SetSkybox(filePaths);
}


void Graphics::SetSkybox(std::string relativeDir, std::string fileName, std::string fileType)
{
	if (skyBoxRenderer == nullptr)
		skyBoxRenderer = new SkyboxRenderer();

	std::vector<std::string> files;
	files.push_back(relativeDir + "\\" + fileName + "_left." + fileType);
	files.push_back(relativeDir + "\\" + fileName + "_right." + fileType);
	files.push_back(relativeDir + "\\" + fileName + "_down." + fileType);
	files.push_back(relativeDir + "\\" + fileName + "_up." + fileType);
	files.push_back(relativeDir + "\\" + fileName + "_front." + fileType);
	files.push_back(relativeDir + "\\" + fileName + "_back." + fileType);

	skyBoxRenderer->SetSkybox(files);
}


void Graphics::SetEnvironment(std::string filePath)
{
	if (skyBoxRenderer == nullptr) {
		skyBoxRenderer = new SkyboxRenderer;
	}

	skyBoxRenderer->SetEnvironment(filePath);
}


void Graphics::OnWindowResize(int width, int height)
{
	if (width && height) {
		glViewport(0, 0, width, height);
		camera->ResetCamera((float)width / (float)height);

		for (auto canvas : canvases) {
			canvas->OnWindowResize();
		}

		RenderFrame();
	}
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
		Lighting::GL_SetLightUniforms();
		//for (auto light : lights) {
		//	light->GL_SetLightingPasses(k++);
		//}

		//std::cout << "binding fucking texture again for this fucking shader: " << shader->name << std::endl;
		//currentlyBoundShader->SetShaderPass(new ShaderPassVec1I(&lightCount, "u_NumberLights"));
		currentlyBoundShader->SetValueVec1I("u_NumberLights", lightCount);
	}
}


void Graphics::RenderShadowPass(Light* light)
{
	//std::cout << "filing fucking buffer" << std::endl;
	for (auto renderer : meshRenderers) {
		renderer->DrawShadowMap(light);
	}
}
