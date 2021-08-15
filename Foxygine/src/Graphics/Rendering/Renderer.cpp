#include "Renderer.h"
#include "../Shaders/ShaderLibrary.h"
#include "Material.h"
#include "../Textures/TextureLibrary.h"
#include "../Graphics.h"



void Renderer::SetMaterial(std::shared_ptr<Material> _material)
{
	material = std::shared_ptr<Material>(_material);
	shader = ShaderLibrary::GetShader(_material->shaderName);

	std::string skyboxName;
	if (Graphics::TryGetSkybox(skyboxName)) {
		auto stuff = std::shared_ptr<Texture>(TextureLibrary::FindTextureByName(skyboxName));
		material->CreateTextureProperty("Skybox", std::shared_ptr<Texture>(TextureLibrary::FindTextureByName(skyboxName)), Material::TextureSlot::Skybox);
		std::cout << "loading skybox" << std::endl;
	}
}

std::shared_ptr<Material> Renderer::GetMaterial()
{
	return std::shared_ptr<Material>(material);
}