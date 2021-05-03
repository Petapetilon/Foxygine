#include "Graphics.h"

std::list<MeshRenderer*> Graphics::meshRenderers;
std::shared_ptr<Camera> Graphics::camera;
long Graphics::renderedFrames;

void Graphics::DrawMeshRenderer() {
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