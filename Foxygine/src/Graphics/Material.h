#pragma once
#include <list>
#include <string>
#include <memory>
#include "Shaders/Shader.h"
#include "Shaders/ShaderLibrary.h"
#include "../Math/Color.h"


class Texture;

class Material
{
enum class TextureSlot;
private:

	struct MaterialProperty {
		std::string propertyName;
		std::string shaderPassName;
		float propertyValue;
	};

	struct TextureProperty {
		std::string propertyName;
		std::shared_ptr<Texture> texture;
		TextureSlot textureSlot;
	};


	std::list<std::shared_ptr<MaterialProperty>> materialProps;
	std::list<std::shared_ptr<TextureProperty>> textureProps;
	std::shared_ptr<Shader> shader;
	Color mainColor;

	std::string textureSlotUniforms[16] = {
		"u_ColorTexture",
		"u_NormalMap",
		"u_DisplacementMap",
		"u_SpecularMap",
		"u_MetallicMap",
		"Custom_1",
		"Custom_2",
		"Custom_3",
		"Custom_4",
		"Custom_5",
		"Custom_6",
		"Custom_7",
		"Custom_8",
		"Custom_9",
		"Custom_10",
		"Custom_11",
	};

public:
	enum class TextureSlot {
		BaseColor,// = 0,
		NormalMap,// = 1,
		DisplacementMap,// = 2,
		Specular,// = 3,
		Metallic,// = 4,
		Custom_1,// = 5,
		Custom_2,// = 6,
		Custom_3,// = 7,
		Custom_4,// = 8,
		Custom_5,// = 9,
		Custom_6,// = 10,
		Custom_7,// = 11,
		Custom_8,// = 12,
		Custom_9,// = 13,
		Custom_10,// = 14,
		Custom_11,// = 15
	};


	std::string shaderName;
	std::string name;

	Material(std::string materialName, std::string shaderName);

	void SetShaderPass(ShaderPass*);
	void SetMainColor(Color);

	void CreateMaterialProperty(std::string propertyName, std::string shaderPassName, float propertyValue);
	void SetMaterialProperty(std::string propertyName, float value);
	void DeleteMaterialProperty(std::string propertyName);

	void CreateTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture, Material::TextureSlot textureType);
	void SetCustomTexturePropertyUniformMapping(Material::TextureSlot textureSlot, std::string uniformName);
	void SetTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture);
	void DeleteTextureProperty(std::string propertyName);

	void GL_SetProperties();
};

