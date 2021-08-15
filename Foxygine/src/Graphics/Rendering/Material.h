#pragma once
#include <list>
#include <string>
#include <memory>
#include "../Shaders/Shader.h"
#include "../Shaders/ShaderLibrary.h"
#include "../../Math/Color.h"


class Texture;
class Texture2D;
class SkyboxRenderer;

class Material
{
protected:
	enum class TextureSlot;
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


	Material();
	std::list<std::shared_ptr<MaterialProperty>> materialProps;
	std::list<std::shared_ptr<TextureProperty>> textureProps;
	std::shared_ptr<Shader> shader;

	std::string textureSlotUniforms[18] = {
		"u_ColorTexture",
		"u_NormalMap",
		"u_DisplacementMap",
		"u_RoughnessMap",
		"u_MetallicMap",
		"u_AOMap",
		"u_ReflectionMap",
		"Custom_3",
		"Custom_4",
		"Custom_5",
		"Custom_6",
		"Custom_7",
		"Custom_8",
		"Custom_9",
		"Custom_10",
		"Custom_11",
		"u_Skybox",
		"u_Environment"
	};

public:
	enum class TextureSlot {
		BaseColor = 0,
		NormalMap = 1,
		DisplacementMap = 2,
		RoughnessMap = 3,
		MetallicMap = 4,
		AOMap = 5,
		ReflectionMap = 6,
		Custom_3 = 7,
		Custom_4 = 8,
		Custom_5 = 9,
		Custom_6 = 10,
		Custom_7 = 11,
		Custom_8 = 12,
		Custom_9 = 13,
		Custom_10 = 14,
		Custom_11 = 15,
		Skybox = 16,
		Environment = 17
	};


	std::string shaderName;
	std::string name;

	Color mainColor;
	float normalMappingStrength;
	Vector2 uvScale;
	Vector2 uvOffset;
	
	Material(std::string materialName, std::string shaderName);

	void LoadAlbedoTexture(std::string filePath);
	void SetAlbedoTexture(std::shared_ptr<Texture2D> texture);
	void SetAlbedoTexture(std::string textureName);

	void LoadNormalMap(std::string filePath);
	void SetNormalMap(std::shared_ptr<Texture2D> texture);
	void SetNormalMap(std::string textureName);

	void CreateMaterialProperty(std::string propertyName, std::string shaderPassName, float propertyValue);
	bool SetMaterialProperty(std::string propertyName, float value);
	float GetMaterialProperty(std::string propertyName);
	void DeleteMaterialProperty(std::string propertyName);

	void CreateTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture, Material::TextureSlot textureType);
	void SetCustomTexturePropertyUniformMapping(Material::TextureSlot textureSlot, std::string uniformName);
	bool SetTextureProperty(std::string propertyName, std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> GetTextureProperty(std::string propertyName);
	void DeleteTextureProperty(std::string propertyName);
	bool TryFindTexturePropertyOfTextureSlot(Material::TextureSlot textureType, std::string& result);

	void FinishLoadingResources();
	void GL_SetProperties();
	std::shared_ptr<Shader> GetShader();
};

