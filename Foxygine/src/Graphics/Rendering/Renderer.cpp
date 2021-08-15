#include "Renderer.h"
#include "../Shaders/ShaderLibrary.h"
#include "Material.h"
#include "../Textures/TextureLibrary.h"
#include "../Graphics.h"



void Renderer::SetMaterial(std::shared_ptr<Material> _material)
{
	material = std::shared_ptr<Material>(_material);
	shader = ShaderLibrary::GetShader(_material->shaderName);

	std::shared_ptr<Texture> skyboxTexture;
	if (Graphics::TryGetSkybox(skyboxTexture)) {
		material->CreateTextureProperty("Skybox", std::shared_ptr<Texture>(skyboxTexture), Material::TextureSlot::Skybox);
		std::cout << "loading skybox" << std::endl;
	}
}

std::shared_ptr<Material> Renderer::GetMaterial()
{
	return std::shared_ptr<Material>(material);
}