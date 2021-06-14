#include "Material.h"
#include "../Shaders/ShaderLibrary.h"
#include "Camera.h"
#include "../Shaders/Shader.h"
#include "../Textures/Texture2D.h"
#include "../../Foxygine Include.h"
#include "../Textures/TextureLibrary.h"


Material::Material()
{
	//In case Shader does not exist
	if (shader == nullptr) {
		std::cout << "Shader not found! Creating Shader with given Name: " << shaderName << std::endl;
		shader = std::shared_ptr<Shader>(Shader::CreateBasicLitShader(shaderName));
	}

	mainColor = Color(1, 1, 1, 1.f);
	uvScale = Vector2(1, 1);
	uvOffset = Vector2(0, 0);
	normalMappingStrength = 1;
}


Material::Material(std::string materialName, std::string _shaderName)
{
	name = materialName;
	shaderName = _shaderName;
	shader = ShaderLibrary::GetShader(shaderName);

	//In case Shader does not exist
	if (shader == nullptr) {
		std::cout << "Shader not found! Creating Shader with given Name: " << shaderName << std::endl;
		shader = std::shared_ptr<Shader>(Shader::CreateBasicLitShader(shaderName));
	}

	mainColor = Color(1, 1, 1, 1.f);
	uvScale = Vector2(1, 1);
	uvOffset = Vector2(0, 0);
	normalMappingStrength = 1;
}


void Material::LoadAlbedoTexture(string filePath)
{
	Texture2D* tex = nullptr;
	if (!TextureLibrary::TryFindTexture(filePath, tex)) {
		tex = new Texture2D();
		tex->LoadTexture2D(filePath, name + "_albedoTex", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
	}

	std::string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
	}
	else {
		propName = name + "_albedoTex";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::BaseColor);
	}
}


void Material::SetAlbedoTexture(std::shared_ptr<Texture2D> texture)
{
	string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
		SetTextureProperty(propName, texture);
	}
	else {
		propName = name + "_albedoTex";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(texture), TextureSlot::BaseColor);
	}
}


void Material::SetAlbedoTexture(std::string textureName)
{
	Texture2D* tex = nullptr;
	if (TextureLibrary::TryFindTextureByName(textureName, tex)) {
		string propName;
		if (TryFindTexturePropertyOfTextureSlot(TextureSlot::BaseColor, propName)) {
			SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
		}
		else {
			propName = name + "_albedoTex";
			CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::BaseColor);
		}
	}
}


void Material::LoadNormalMap(std::string filePath)
{
	Texture2D* tex = nullptr;
	if (!TextureLibrary::TryFindTexture(filePath, tex)) {
		tex = new Texture2D();
		tex->LoadTexture2D(filePath, name + "_normalMap", Texture::Wrapping::Repeat, Texture::Filtering::Linear);
	}

	string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::NormalMap, propName)) {
		SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
	}
	else {
		propName = name + "_normalMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::NormalMap);
	}
}


void Material::SetNormalMap(std::shared_ptr<Texture2D> texture)
{
	string propName;
	if (TryFindTexturePropertyOfTextureSlot(TextureSlot::NormalMap, propName)) {
		SetTextureProperty(propName, texture);
	}
	else {
		propName = name + "_normalMap";
		CreateTextureProperty(propName, std::shared_ptr<Texture>(texture), TextureSlot::NormalMap);
	}
}


void Material::SetNormalMap(std::string textureName)
{
	Texture2D* tex = nullptr;
	if (TextureLibrary::TryFindTextureByName(textureName, tex)) {
		string propName;
		if (TryFindTexturePropertyOfTextureSlot(TextureSlot::NormalMap, propName)) {
			SetTextureProperty(propName, std::shared_ptr<Texture>(tex));
		}
		else {
			propName = name + "_normalMap";
			CreateTextureProperty(propName, std::shared_ptr<Texture>(tex), TextureSlot::NormalMap);
		}
	}
}


void Material::CreateMaterialProperty(std::string propertyName, std::string shaderPassName, float propertyValue)
{
	for (auto prop : materialProps) {
		if (prop->propertyName == propertyName) {
			std::cout << "Can not create new MaterialProperty: " << propertyName << "already exists!" << std::endl;
			return;
		}
	}

	auto newProp = new MaterialProperty();
	newProp->propertyName = propertyName;
	newProp->shaderPassName = shaderPassName;
	newProp->propertyValue = propertyValue;
	materialProps.push_back(std::shared_ptr<MaterialProperty>(newProp));
}


bool Material::SetMaterialProperty(std::string propertyName, float value)
{
	for (auto prop : materialProps) {
		if (prop->propertyName == propertyName) {
			prop->propertyValue = value;
			return true;
		}
	}

	return false;
}


void Material::DeleteMaterialProperty(std::string propertyName)
{
	for (auto prop : materialProps) {
		if (prop->propertyName == propertyName) {
			materialProps.remove(prop);
		}
	}
}


void Material::CreateTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture, TextureSlot textureType)
{
	for (auto prop : textureProps) {
		if (prop->propertyName == propertyName) {
			std::cout << "Can not create new TextureProperty: " << propertyName << "already exists!" << std::endl;
			return;
		} 
		else if (prop->textureSlot == textureType) {
			std::cout << "Can not create another TextureProperty of Type: " << (int)textureType << std::endl;
			return;
		}
	}

	auto newProp = new TextureProperty();
	newProp->propertyName = propertyName;
	newProp->texture = std::shared_ptr<Texture>(texture);
	newProp->textureSlot = textureType;
	textureProps.push_back(std::shared_ptr<TextureProperty>(newProp));
	newProp->texture->GL_GetUniform(std::shared_ptr<Shader>(shader), textureSlotUniforms[(unsigned int)newProp->textureSlot]);
}


bool Material::SetTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture)
{
	for (auto prop : textureProps) {
		if (prop->propertyName == propertyName) {
			prop->texture = std::shared_ptr<Texture>(texture);
			prop->texture->GL_GetUniform(std::shared_ptr<Shader>(shader), textureSlotUniforms[(unsigned int)prop->textureSlot]);
			return true;
		}
	}

	return false;
}


void Material::SetCustomTexturePropertyUniformMapping(Material::TextureSlot textureSlot, std::string uniformName)
{
	textureSlotUniforms[(int)textureSlot] = uniformName;
}


void Material::DeleteTextureProperty(std::string propertyName)
{
	for (auto prop : textureProps) {
		if (prop->propertyName == propertyName) {
			textureProps.remove(prop);
			return;
		}
	}
}


bool Material::TryFindTexturePropertyOfTextureSlot(Material::TextureSlot textureType, std::string& result)
{
	for (auto prop : textureProps) {
		if (prop->textureSlot == textureType) {
			result = prop->propertyName;
			return true;
		}
	}

	return false;
}


void Material::FinishLoadingResources()
{
	for (auto texProp : textureProps) {
		texProp->texture->FinishLoading();
	}
}


void Material::GL_SetProperties()
{
	//shader->SetShaderPass(new ShaderPassColor(&mainColor, "u_MaterialProps.color"));
	GL_Call(glUniform4f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_MaterialProps.color"), mainColor.r, mainColor.g, mainColor.b, mainColor.a));
	GL_Call(glUniform2f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_MaterialProps.uvScale"), uvScale.x, uvScale.y));
	GL_Call(glUniform2f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_MaterialProps.uvOffset"), uvOffset.x, uvOffset.y));

	for (auto prop : materialProps) {
		//shader->SetShaderPass(new ShaderPassVec1(&prop->propertyValue, "u_MaterialProps." + prop->shaderPassName));
		GL_Call(glUniform1f(glGetUniformLocation(shader->GL_GetShaderProgram(), ("u_MaterialProps." + prop->shaderPassName).c_str()), prop->propertyValue));
	}

	int ColEnable = 0;
	float NormEnable = 0;
	int DispEnable = 0;
	int SpecEnable = 0;
	int MetEnable = 0;


	for (auto prop : textureProps) {
		prop->texture->GL_BindTexture((unsigned int)prop->textureSlot);
		switch (prop->textureSlot)
		{
		case TextureSlot::BaseColor:
			ColEnable = 1;
			break;
			
		case TextureSlot::NormalMap:
			NormEnable = normalMappingStrength;
			break;
			
		case TextureSlot::DisplacementMap:
			DispEnable = 1;
			break;
			
		case TextureSlot::RoughnessMap:
			SpecEnable = 1;
			break;
			
		case TextureSlot::MetallicMap:
			MetEnable = 1;
			break;

		default:
			break;
		}
	}


	GL_Call(glUniform1i(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_ColTexEnabled"), ColEnable));
	GL_Call(glUniform1f(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_NormTexEnabled"), NormEnable));
	GL_Call(glUniform1i(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_DispTexEnabled"), DispEnable));
	GL_Call(glUniform1i(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_RghTexEnabled"), SpecEnable));
	GL_Call(glUniform1i(glGetUniformLocation(shader->GL_GetShaderProgram(), "u_MetTexEnabled"), MetEnable));
}


std::shared_ptr<Shader> Material::GetShader()
{
	return std::shared_ptr<Shader>(shader);
}
