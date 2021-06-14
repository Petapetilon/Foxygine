#include "PBRMaterial.h"
#include "../Textures/TextureLibrary.h"
#include "../../Foxygine Include.h"


PBRMaterial::PBRMaterial()
{
	name = "PBRMaterial";
	shaderName = "Internal_PBR_Shader";
	shader = ShaderLibrary::GetShader("Internal_PBR_Shader");

	//In case Shader does not exist
	if (shader == nullptr) {
		std::cout << "Shader not found! Creating Shader with given Name: " << shaderName << std::endl;
		shader = std::shared_ptr<Shader>(Shader::CreatePBRShader(shaderName));
	}

	mainColor = Color(1, 1, 1, 1.f);
	uvScale = Vector2(1, 1);
	uvOffset = Vector2(0, 0);
	normalMappingStrength = 1;

	CreateMaterialProperty("roughness", "roughness", .5f);
	CreateMaterialProperty("metallic", "metallic", .5f);
}


PBRMaterial::PBRMaterial(std::string _name)
{
	name = _name;
	shaderName = "Internal_PBR_Shader";
	shader = ShaderLibrary::GetShader("Internal_PBR_Shader");

	//In case Shader does not exist
	if (shader == nullptr) {
		std::cout << "Shader not found! Creating Shader with given Name: " << shaderName << std::endl;
		shader = std::shared_ptr<Shader>(Shader::CreatePBRShader(shaderName));
	}

	mainColor = Color(1, 1, 1, 1.f);
	uvScale = Vector2(1, 1);
	uvOffset = Vector2(0, 0);
	normalMappingStrength = 1;

	CreateMaterialProperty("roughness", "roughness", .5f);
	CreateMaterialProperty("metallic", "metallic", .5f);
}


void PBRMaterial::SetRoughness(float value)
{
	SetMaterialProperty("roughness", value);
}


void PBRMaterial::SetMetallic(float value)
{
	SetMaterialProperty("metallic", value);
}


void PBRMaterial::LoadRoughnessMap(string filePath)
{
	Texture2D* tex = nullptr;
	if (!TextureLibrary::TryFindTexture(filePath, tex)) {
		tex = new Texture2D();
		tex->LoadTexture2D(filePath, name + "_RoughnessMap", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
	}

	std::string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
	}
	else {
		propName = name + "_RoughnessMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::RoughnessMap);
	}
}


void PBRMaterial::SetRoughnessMap(std::shared_ptr<Texture2D> texture)
{
	string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, texture);
	}
	else {
		propName = name + "_RoughnessMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(texture), TextureSlot::RoughnessMap);
	}
}


void PBRMaterial::SetRoughnessMap(std::string textureName)
{
	Texture2D* tex = nullptr;
	if (TextureLibrary::TryFindTextureByName(textureName, tex)) {
		string propName;
		if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
			SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
		}
		else {
			propName = name + "_RoughnessMap";
			CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::RoughnessMap);
		}
	}
}


void PBRMaterial::LoadMetallicMap(string filePath)
{
	Texture2D* tex = nullptr;
	if (!TextureLibrary::TryFindTexture(filePath, tex)) {
		tex = new Texture2D();
		tex->LoadTexture2D(filePath, name + "_MetallicMap", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
	}

	std::string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
	}
	else {
		propName = name + "_MetallicMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::MetallicMap);
	}
}


void PBRMaterial::SetMetallicMap(std::shared_ptr<Texture2D> texture)
{
	string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, texture);
	}
	else {
		propName = name + "_MetallicMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(texture), TextureSlot::MetallicMap);
	}
}


void PBRMaterial::SetMetallicMap(std::string textureName)
{
	Texture2D* tex = nullptr;
	if (TextureLibrary::TryFindTextureByName(textureName, tex)) {
		string propName;
		if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
			SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
		}
		else {
			propName = name + "_MetallicMap";
			CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::MetallicMap);
		}
	}
}


void PBRMaterial::LoadAOMap(string filePath)
{
	Texture2D* tex = nullptr;
	if (!TextureLibrary::TryFindTexture(filePath, tex)) {
		tex = new Texture2D();
		tex->LoadTexture2D(filePath, name + "_AOMap", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
	}

	std::string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
	}
	else {
		propName = name + "_AOMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::AOMap);
	}
}


void PBRMaterial::SetAOMap(std::shared_ptr<Texture2D> texture)
{
	string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, texture);
	}
	else {
		propName = name + "_AOMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(texture), TextureSlot::AOMap);
	}
}


void PBRMaterial::SetAOMap(std::string textureName)
{
	Texture2D* tex = nullptr;
	if (TextureLibrary::TryFindTextureByName(textureName, tex)) {
		string propName;
		if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
			SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
		}
		else {
			propName = name + "_AOMap";
			CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::AOMap);
		}
	}
}
