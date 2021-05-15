#include "Renderer.h"
#include "Shaders/ShaderLibrary.h"
#include "Camera.h"
#include "SerializedMesh.h"
#include "Material.h"



void Renderer::SetMaterial(std::shared_ptr<Material> _material)
{
	material = std::shared_ptr<Material>(_material);
	shader = ShaderLibrary::GetShader(_material->shaderName);
}

std::shared_ptr<Material> Renderer::GetMaterial()
{
	return std::shared_ptr<Material>(material);
}