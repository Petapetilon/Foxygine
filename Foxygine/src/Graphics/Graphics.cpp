#include "Graphics.h"
#include <thread>


std::shared_ptr<GameObject> Graphics::skyBoxGo;
std::list<MeshRenderer*> Graphics::meshRenderers;
std::shared_ptr<Camera> Graphics::camera;
long Graphics::renderedFrames;
unsigned int Graphics::GL_CurrentlyBoundShaderProgram;


void Graphics::DrawRenderer() {
	for (const auto& renderer : meshRenderers) {
		renderer->Draw(camera);
	}

	skyBoxGo->GetComponent<SkyboxRenderer>()->Draw(camera);
	renderedFrames++;
}


void Graphics::RegisterMeshRenderer(MeshRenderer* meshRenderer) {
	meshRenderers.push_back(meshRenderer);
}


void Graphics::UnregeisterMeshRenderer(MeshRenderer* meshRenderer) {
	meshRenderers.remove(meshRenderer);
}


void Graphics::SetSkybox(std::vector<std::string> filePaths)
{
	skyBoxGo = GameObject::CreateGameObject("skyBoxGo");
	skyBoxGo->AddComponent<SkyboxRenderer>(new SkyboxRenderer());
	skyBoxGo->GetComponent<SkyboxRenderer>()->SetSkybox(filePaths);
}

void Graphics::OnWindowResize(int width, int height)
{
	camera->ResetCamera(width / height);
	glViewport(0, 0, width, height);
	DrawRenderer();
}

void Graphics::SetLightingData()
{
}
