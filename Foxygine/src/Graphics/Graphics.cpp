#include "Graphics.h"
#include <thread>


std::list<MeshRenderer*> Graphics::meshRenderers;
std::shared_ptr<Camera> Graphics::camera;
long Graphics::renderedFrames;
unsigned int Graphics::GL_CurrentlyBoundShaderProgram;


void Graphics::DrawRenderer() {
	for (const auto& renderer : meshRenderers) {
		renderer->Draw(camera);
	}

	renderedFrames++;
}


void Graphics::RegisterMeshRenderer(MeshRenderer* meshRenderer) {
	meshRenderers.push_back(meshRenderer);
}


void Graphics::UnregeisterMeshRenderer(MeshRenderer* meshRenderer) {
	meshRenderers.remove(meshRenderer);
}
