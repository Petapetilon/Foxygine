#include "Renderer.h"
#include "Shaders/ShaderLibrary.h"



void Renderer::SetMaterial(std::shared_ptr<Material> _material)
{
	material = std::shared_ptr<Material>(_material);
	shader = ShaderLibrary::GetShader(_material->shaderName);
	std::cout << "Setting material in MeshRenderer: " << material->name << std::endl;
}

std::shared_ptr<Material> Renderer::GetMaterial()
{
	return std::shared_ptr<Material>(material);
}