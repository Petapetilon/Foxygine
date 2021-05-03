#include "Material.h"
#include "Shaders/ShaderLibrary.h"
#include "Shaders/ShaderPass.h"
#include "Camera.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"


Material::Material(std::string materialName, std::string _shaderName)
{
	name = materialName;
	shaderName = _shaderName;
	shader = ShaderLibrary::GetShader(shaderName);

	//In case Shader does not exist
	if (shader == nullptr) {
		std::cout << "Shader not found! Creating Shader with given Name: " << shaderName << std::endl;
		shader = std::unique_ptr<Shader>(new Shader(shaderName));
	}
}


void Material::SetShaderPass(ShaderPass* shaderPass)
{
	shader->SetShaderPass(shaderPass);
}


void Material::SetMainColor(Color color)
{
	mainColor.a = color.a;
	mainColor.r = color.r;
	mainColor.g = color.g;
	mainColor.b = color.b;
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


void Material::SetMaterialProperty(std::string propertyName, float value)
{
	for (auto prop : materialProps) {
		if (prop->propertyName == propertyName) {
			prop->propertyValue = value;
		}
	}
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


void Material::SetTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture)
{
	for (auto prop : textureProps) {
		if (prop->propertyName == propertyName) {
			prop->texture = std::shared_ptr<Texture>(texture);
			prop->texture->GL_GetUniform(std::shared_ptr<Shader>(shader), textureSlotUniforms[(unsigned int)prop->textureSlot]);
			return;
		}
	}
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


void Material::GL_SetProperties()
{
	shader->SetShaderPass(new ShaderPassColor(&mainColor, "u_MaterialProps.color"));

	for (auto prop : materialProps) {
		shader->SetShaderPass(new ShaderPassVec1(&prop->propertyValue, "u_MaterialProps." + prop->shaderPassName));
	}

	int ColEnable = false;
	int NormEnable = false;
	int DispEnable = false;
	int SpecEnable = false;
	int MetEnable = false;


	for (auto prop : textureProps) {
		prop->texture->GL_BindTexture((unsigned int)prop->textureSlot);
		switch (prop->textureSlot)
		{
		case TextureSlot::BaseColor:
			ColEnable = true;
			break;
			
		case TextureSlot::NormalMap:
			NormEnable = true;
			break;
			
		case TextureSlot::DisplacementMap:
			DispEnable = true;
			break;
			
		case TextureSlot::Specular:
			SpecEnable = true;
			break;
			
		case TextureSlot::Metallic:
			MetEnable = true;
			break;

		default:
			break;
		}
	}

	shader->SetShaderPass(new ShaderPassVec1I(&ColEnable, "u_ColTexEnabled"));
	shader->SetShaderPass(new ShaderPassVec1I(&NormEnable, "u_NormTexEnabled"));
	shader->SetShaderPass(new ShaderPassVec1I(&DispEnable, "u_DispTexEnabled"));
	shader->SetShaderPass(new ShaderPassVec1I(&SpecEnable, "u_SpecTexEnabled"));
	shader->SetShaderPass(new ShaderPassVec1I(&MetEnable, "u_MetTexEnabled"));
}